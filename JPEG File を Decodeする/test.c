#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Marker code */
#define SOF0 (0xC0) /* Baseline DCT                         */
#define DHT  (0xC4) /* Define Huffman table(s)              */
#define SOS  (0xDA) /* Start of scan                        */
#define DQT  (0xDB) /* Define quantization table(s)         */

/* Frame header */
struct sofn_c_t {
  uint8_t C;    /* Component identifier                     */
  uint8_t V:4;  /* Vertical sampling factor                 */
  uint8_t H:4;  /* Horizontal sampling factor               */
  uint8_t Tq;   /* Quantization table destination selector  */
} __attribute__((packed));

struct sofn_t {
  uint16_t Lf;  /* Frame header length                      */
  uint8_t P;    /* Sample precision                         */
  uint16_t Y;   /* Number of lines                          */
  uint16_t X;   /* Number of samples per line               */
  uint8_t Nf;   /* Number of image components in frame      */
#define MAX_NF (3)
  struct sofn_c_t C[MAX_NF];
  uint8_t Hmax;
  uint8_t Vmax;
} __attribute__((packed));

/* Scan header */
struct sos_c_t {
  uint8_t Cs;   /* Scan component selector                      */
  uint8_t Ta:4; /* AC entropy coding table destination selector */
  uint8_t Td:4; /* DC entropy coding table destination selector */
} __attribute__((packed));

struct sos_t {
  uint16_t Ls;  /* Scan header length                           */
  uint8_t Ns;   /* Number of image components in scan           */
#define MAX_NS (3)
  struct sos_c_t C[MAX_NS];
  uint8_t Ss;   /* Start of spectral or predictor selection     */
  uint8_t Se;   /* End of spectral selection                    */
  uint8_t Al:4; /* Successive approximation bit position low or point transform */
  uint8_t Ah:4; /* Successive approximation bit position high                   */
} __attribute__((packed));

/* Quantization table */
#define PQ_8BIT  (0)
struct dqt_t {
  uint16_t Lq;  /* Quantization table definition length       */
  uint8_t Tq:4; /* Quantization table destination identifier  */
  uint8_t Pq:4; /* Quantization table element precision       */
#define Q_LEN (128)
  uint8_t Q[Q_LEN];
} __attribute__((packed));

/* Huffman table */
struct dht_t {
  uint16_t Lh;      /* Huffman table definition length                              */
  uint8_t Th:4;     /* Huffman table destination identifier                         */
  uint8_t Tc:4;     /* Table class - 0 = DC table or lossless table, 1 = AC table.  */
#define L_LEN (16)
  uint8_t L[L_LEN]; /* Number of Huffman codes of length i                          */
#define V_LEN (256)
  uint8_t V[V_LEN]; /* Value associated with each Huffman code                      */
  uint8_t HUFFSIZE[V_LEN];
  uint8_t last;
  uint16_t HUFFCODE[V_LEN];
  uint8_t VALPTR[V_LEN];
  int MINCODE[V_LEN];
  int MAXCODE[V_LEN];
} __attribute__((packed));

struct info_c_t {
  int dc_pred;
  uint8_t* component;
  int width;
  int height;
} __attribute__((packed));

struct info_t {
  struct info_c_t C[MAX_NF];
} __attribute__((packed));

static FILE* g_fp;
static struct info_t g_info;

static int decode(struct dht_t* dht, int* code, int* size);
static int extend(int V, int T);
static int getbytes(uint8_t* buf, int size);
static int nextbit();
static int receive(int ssss);

static const int zig_zag_order[][8] = {
  {  0,  1,  5,  6, 14, 15, 27, 28 },
  {  2,  4,  7, 13, 16, 26, 29, 42 },
  {  3,  8, 12, 17, 25, 30, 41, 43 },
  {  9, 11, 18, 24, 31, 40, 44, 53 },
  { 10, 19, 23, 32, 39, 45, 52, 54 },
  { 20, 22, 33, 38, 46, 51, 55, 60 },
  { 21, 34, 37, 47, 50, 56, 59, 61 },
  { 35, 36, 48, 49, 57, 58, 62, 63 },
};

static void IDCT(int F[][8], int f[][8])
{
  int x,y,u,v;
  double N=8.0;

  for (y=0; y<8; y++) {
    for (x=0; x<8; x++) {
      double val = 0;
      for (v=0; v<8; v++) {
        for (u=0; u<8; u++) {
          double cu,cv;
          if (u == 0) {
            cu = 0.707106781; // 1/2^(1/2)
          } else {
            cu = 1.0;
          }
          if (v == 0) {
            cv = 0.707106781; // 1/2^(1/2)
          } else {
            cv = 1.0;
          }
          val += cu*cv*F[v][u]*cos((2.0*x+1.0)*(double)u*M_PI/(2.0*N))*cos((2.0*y+1.0)*(double)v*M_PI/(2.0*N));
        }
      }
      f[y][x] = round((2.0/N)*val);
    }
  }
}

static void print_binary(uint16_t value, int len)
{
  int i;

  for (i=0; i<len && i<16; i++) {
    int shift = len - i - 1;
    printf("%d", (value>>shift)&1);
  }
}

static void swap2byte(uint8_t* pbuf)
{
  uint8_t tmp;
  tmp = pbuf[1];
  pbuf[1] = pbuf[0];
  pbuf[0] = tmp;
}

static int getbytes(uint8_t* buf, int size)
{
  size_t sz = fread(buf, 1, size, g_fp);
  if (sz != size) {
    if (feof(g_fp)) {
      return 1; /* EOF */
    } else {
      return -1;
    }
  }

  return 0;
}

static void print_frame_header(struct sofn_t* sofn)
{
  int i;

  printf("SOF:\n");
  printf("  Lf=%d\n", sofn->Lf);
  printf("  P =%d\n", sofn->P);
  printf("  Y =%u\n", sofn->Y);
  printf("  X =%u\n", sofn->X);
  printf("  Nf=%u\n", sofn->Nf);
  for (i=0; i<sofn->Nf; i++) {
    printf("    C(%d) H %d V %d Tq %d \n", sofn->C[i].C, sofn->C[i].H, sofn->C[i].V, sofn->C[i].Tq);
  }
  printf("  Hmax=%u Vmax=%u\n", sofn->Hmax, sofn->Vmax);
  printf("\n");
}

static bool is_known_marker(uint8_t marker)
{
  if ((marker == SOF0) || (marker == DHT ) || (marker == SOS ) || (marker == DQT )) {
    return true;
  } else {
    return false;
  }
}

static uint16_t next_marker()
{
  int ret;
  uint16_t marker;

  while (1) {
    ret = getbytes((uint8_t*)&marker, 2);
    if (ret != 0) {
      return ret;
    }
    if ((marker & 0x00FF) == 0xFF && (is_known_marker(marker>>8))) {
      break;
    }
  }

  return marker>>8;
}

static void parse_frame_header(struct sofn_t* sofn)
{
  int ret, offset = 0, i;
  uint16_t marker;
  uint8_t *pbuf = (uint8_t*)sofn;

  marker = next_marker();
  if (marker != SOF0) {
    printf("error parse_frame_header %d\n", marker);
    return;
  }

  memset(pbuf, 0, sizeof(struct sofn_t));

  ret = getbytes(pbuf+offset, 2);
  if (ret != 0) {
    return;
  }
  swap2byte(pbuf+offset);
  offset += 2;

  ret = getbytes(pbuf+offset, sofn->Lf - 2);
  if (ret != 0) {
    return;
  }

  /* P */
  offset += 1;
  /* Y */
  swap2byte(pbuf+offset);
  offset += 2;
  /* X */
  swap2byte(pbuf+offset);
  offset += 2;
  /* Nf */
  offset += 1;
  /* calc Hmax Vmax */
  for (i=0; i<sofn->Nf && i<MAX_NF; i++) {
    if (sofn->Hmax < sofn->C[i].H) {
      sofn->Hmax = sofn->C[i].H;
    }
    if (sofn->Vmax < sofn->C[i].V) {
      sofn->Vmax = sofn->C[i].V;
    }
  }
}

static void print_quantization_table(struct dqt_t* dqt)
{
  int i;

  printf("DQT:\n");
  printf("  Lq=%d\n", dqt->Lq);
  printf("  Pq=%d(%s)\n", dqt->Pq, dqt->Pq == 0 ? "8bit" : "16bit");
  printf("  Tq=%d\n", dqt->Tq);
  printf("  Q\n  ");
  if (dqt->Pq == PQ_8BIT) {
    for (i=0; i<Q_LEN/2; i++) {
      printf("%03d ", dqt->Q[i]);
    }
  }
  printf("\n\n");
}

static void parse_quantization_table(struct dqt_t* dqt)
{
  int ret, offset = 0;
  uint16_t marker;
  uint8_t *pbuf = (uint8_t*)dqt;

  marker = next_marker();
  if (marker != DQT) {
    printf("error parse_quantization_table %d\n", marker);
  }

  memset(pbuf, 0, sizeof(struct dqt_t));

  ret = getbytes(pbuf+offset, 2);
  if (ret != 0) {
    return;
  }
  swap2byte(pbuf+offset);
  offset += 2;

  ret = getbytes(pbuf+offset, dqt->Lq - 2);
  if (ret != 0) {
    return;
  }
  offset += dqt->Lq - 2;
}

static void print_scan_header(struct sos_t* sos)
{
  int i;

  printf("SOS:\n");
  printf("  Ls=%d\n", sos->Ls);
  printf("  Ns=%d\n", sos->Ns);
  printf("  Ss=%d\n", sos->Ss);
  printf("  Se=%d\n", sos->Se);
  for (i=0; i<sos->Ns && i<MAX_NS; i++) {
    printf("  C(%d) Td=%d Ta=%d\n", sos->C[i].Cs, sos->C[i].Td, sos->C[i].Ta);
  }
  printf("  Ah=%d\n", sos->Ah);
  printf("  Al=%d\n", sos->Al);
  printf("\n");
}

static void generate_size_table(struct dht_t* dht)
{
  int k = 0 , i = 1, j = 1;

  while (1) {
    if (dht->L[i-1] < j && i-1 < L_LEN) {
      i = i + 1;
      j = 1;

      if (16 < i) {
        dht->HUFFSIZE[k] = 0;
        dht->last = k;
        break;
      }
    } else if (k < V_LEN) {
      dht->HUFFSIZE[k] = i;
      k = k + 1;
      j = j + 1;
    } else {
      printf("error fatal generate_size_table\n");
      break; /* error */
    }
  }
}

static void generate_code_table(struct dht_t* dht)
{
  int k = 0, code = 0, si = dht->HUFFSIZE[0];

  while (1) {
    dht->HUFFCODE[k] = code;
    code = code + 1;
    k = k + 1;

    if (dht->HUFFSIZE[k] != si) {
      if (dht->HUFFSIZE[k] == 0) {
        break;
      } else {
        do {
          code = code << 1;
          si = si + 1;
        } while (dht->HUFFSIZE[k] != si);
      }
    }
  }
}

static int decoder_tables(struct dht_t* dht)
{
  int i = 0, j = 0;

  for (i=0; i<16; i++) {
    if (dht->L[i] == 0) {
      dht->MAXCODE[i] = -1;
    } else {
      dht->VALPTR[i] = j;
      dht->MINCODE[i] = dht->HUFFCODE[j];
      j = j + dht->L[i] - 1;
      dht->MAXCODE[i] = dht->HUFFCODE[j];
      j = j + 1;
    }
  }

  return 0;
}

static void parse_huffman_table(struct dht_t* dht)
{
  int ret, offset = 0;
  uint16_t marker;
  uint8_t *pbuf = (uint8_t*)dht;

  marker = next_marker();
  if (marker != DHT) {
    printf("error parse_huffman_table %d\n", marker);
    return;
  }

  memset(pbuf, 0, sizeof(struct dht_t));

  ret = getbytes(pbuf+offset, 2);
  if (ret != 0) {
    return;
  }
  swap2byte(pbuf+offset);
  offset += 2;

  ret = getbytes(pbuf+offset, dht->Lh - 2);
  if (ret != 0) {
    return;
  }
  offset += dht->Lh - 2;

  generate_size_table(dht);
  generate_code_table(dht);
  decoder_tables(dht);
}

static void print_huffman_table(struct dht_t* dht)
{
  int i, sum = 0;

  printf("DHT:\n");
  printf("  Lh=%d\n", dht->Lh);
  printf("  Tc=%d(%s)\n", dht->Tc, dht->Tc==0 ? "DC" : "AC");
  printf("  Th=%d\n", dht->Th);
  printf("  L(BITS)\n  ");
  for (i=0; i<L_LEN; i++) {
    sum += dht->L[i];
    printf("%03d ", dht->L[i]);
  }
  printf("\n");

  printf("  HUFFSIZE last=%d\n  ", dht->last);
  for (i=0; i<dht->last; i++) {
    printf("%03d ", dht->HUFFSIZE[i]);
    if ((i+1)%16 == 0 && (i+1) != dht->last)
      printf("\n  ");
  }
  printf("\n");

  printf("  HUFFVAL(V) HUFFCODE\n");
  for (i=0; i<dht->last; i++) {
    printf("  %02X         ", dht->V[i]);
    print_binary(dht->HUFFCODE[i], dht->HUFFSIZE[i]);
    printf("\n");
  }

  printf("  MINCODE\n  ");
  for (i=0; i<16; i++) {
    printf("%d ", dht->MINCODE[i]);
  }
  printf("\n");

  printf("  MAXCODE\n  ");
  for (i=0; i<16; i++) {
    printf("%d ", dht->MAXCODE[i]);
  }
  printf("\n");

  printf("  VALPTR\n  ");
  for (i=0; i<16; i++) {
    printf("%d ", dht->VALPTR[i]);
  }
  printf("\n\n");
}

static void parse_scan_header(struct sos_t* sos)
{
  int ret, offset = 0;
  uint16_t marker;
  uint8_t *pbuf = (uint8_t*)sos;

  marker = next_marker();
  if (marker != SOS) {
    printf("error parse_scan_header %d\n", marker);
    return;
  }

  memset(pbuf, 0, sizeof(struct sos_t));

  ret = getbytes(pbuf+offset, 2);
  if (ret != 0) {
    return;
  }
  swap2byte(pbuf+offset);
  offset += 2;

  ret = getbytes(pbuf+offset, sos->Ls - 2);
  if (ret != 0) {
    return;
  }
  offset += sos->Ls - 2;
}

static void decode_ac_coefficients(struct dht_t* dht, int* ZZ)
{
  int K = 0, code, size;

  while (1) {
    int RS = decode(dht, &code, &size);
    int SSSS = RS % 16;
    int RRRR = RS >> 4;
    int R = RRRR;

    if (SSSS == 0) {
      if (R == 15) {
        K += 16;
        if (62 <= K) {
          break;
        }
      } else {
        break;
      }
    } else {
      K += R;
      ZZ[K] = receive(SSSS);
      ZZ[K] = extend(ZZ[K], SSSS);
      if (62 <= K) {
        break;
      } else {
        K += 1;
      }
    }
  }
}

static int decode_dc_coefficient(struct dht_t* dht)
{
  int code, size;
  int T = decode(dht, &code, &size);
  int DIFF = receive(T);
  return extend(DIFF, T);
}

static void decode_entropy_coded_data_unit(int index, struct dht_t* dc, struct dht_t* ac, int* ZZ)
{
  ZZ[0] = decode_dc_coefficient(dc) + g_info.C[index].dc_pred;
  g_info.C[index].dc_pred = ZZ[0];
  decode_ac_coefficients(ac, &ZZ[1]);
}

static uint8_t nextbyte()
{
  int ret;
  uint8_t val = 0;

  ret = getbytes(&val, 1);
  if (ret != 0) {
    exit(0);
  }
  return val;
}

static void level_shift(int f[8][8])
{
  int i,j;

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      f[i][j] += 128;
      if (f[i][j] < 0) {
        f[i][j] = 0;
      }
      if (255 < f[i][j]) {
        f[i][j] = 255;
      }
    }
  }
}

static void rearrange_zig_zag_order(int* ZZ, int F[8][8])
{
  int i,j;

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      F[i][j] = ZZ[zig_zag_order[i][j]];
    }
  }
}

static void dequantize(struct dqt_t* dqt, int* ZZ)
{
  int i;

  for (i=0; i<64; i++) {
    ZZ[i] *= dqt->Q[i];
  }
}

static void copy_data_unit(int dux, int duy, int index, int f[8][8])
{
  int i,j;
  struct info_c_t* C = g_info.C;
  int stride = C[index].width;
  uint8_t* component = C[index].component;
  int offset = duy * 8 * stride + dux * 8;

  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      uint8_t* p = component + offset + stride * i;
      p[j] = f[i][j];
    }
  }
}

static void decode_mcu_component(int mcux, int mcuy, int index, uint8_t H, uint8_t V, struct dht_t* dc, struct dht_t* ac, struct dqt_t* dqt)
{
  int row, col;
  int ZZ[64];
  int F[8][8];
  int f[8][8];

  for (row=0; row<V; row++) {
    for (col=0; col<H; col++) {
      memset(ZZ, 0, sizeof(ZZ));
      decode_entropy_coded_data_unit(index, dc, ac, ZZ);
      dequantize(dqt, ZZ);
      rearrange_zig_zag_order(ZZ, F);
      IDCT(F, f);
      level_shift(f);
      copy_data_unit(mcux*H+col, mcuy*V+row, index, f);
    }
  }
}

static struct dht_t* get_dht(int index, struct dht_t* dht, int dht_size, bool is_dc)
{
  int i;

  for (i=0; i<dht_size; i++) {
    if (is_dc) {
      if ((dht[i].Tc == 0 /* DC */) && (dht[i].Th == index)) {
        return &dht[i];
      }
    } else {
      if ((dht[i].Tc == 1 /* AC */) && (dht[i].Th == index)) {
        return &dht[i];
      }
    }
  }

  return NULL;
}

static struct dqt_t* get_dqt(int index, struct dqt_t* dqt, int dqt_size)
{
  int i;

  for (i=0; i<dqt_size; i++) {
    if (dqt[i].Tq == index) {
      return &dqt[i];
    }
  }

  return NULL;
}

static void decode_mcu(int mcux, int mcuy, struct sofn_t* sofn, struct sos_t* sos, struct dht_t* dht, int dht_size, struct dqt_t* dqt, int dqt_size)
{
  int i;
  struct dht_t* dht_dc;
  struct dht_t* dht_ac;
  struct dqt_t* _dqt;

  for (i=0; i<sofn->Nf; i++) {
    int Td = sos->C[i].Td;
    int Ta = sos->C[i].Ta;
    int Tq = sofn->C[i].Tq;
    dht_dc = get_dht(Td, dht, dht_size, true  /* dc */);
    dht_ac = get_dht(Ta, dht, dht_size, false /* ac */);
    _dqt = get_dqt(Tq, dqt, dqt_size);
    decode_mcu_component(mcux, mcuy, i, sofn->C[i].H, sofn->C[i].V, dht_dc, dht_ac, _dqt);
  }
}

static void print_component(int index)
{
  uint8_t* component = g_info.C[index].component;
  int w = g_info.C[index].width;
  int h = g_info.C[index].height;
  int i,j;

  printf("P2\n");
  printf("%d %d\n", w, h);
  printf("255\n");
  for (i=0; i<h; i++) {
    for (j=0; j<w; j++) {
      printf("%3d ", component[w*i+j]);
    }
    printf("\n");
  }
  printf("\n");
}

static void decode_entropy_coded_segment(struct sofn_t* sofn, struct sos_t* sos, struct dht_t* dht, int dht_size, struct dqt_t* dqt, int dqt_size)
{
  int row, col, i;
  int xf = sofn->Hmax<<3; /* x8 */
  int yf = sofn->Vmax<<3;
  int mcux = (sofn->X+xf-1)/xf;
  int mcuy = (sofn->Y+yf-1)/yf;
  int x = mcux*xf;
  int y = mcuy*yf;

  printf("X=%d Y=%d Hmax=%d Vmax=%d x=%d y=%d mcux=%d mcuy=%d xf=%d yf=%d\n\n",
    sofn->X, sofn->Y, sofn->Hmax, sofn->Vmax, x, y, mcux, mcuy, xf, yf);

  for (i=0; i<sofn->Nf; i++) {
    int w = mcux * sofn->C[i].H * 8;
    int h = mcuy * sofn->C[i].V * 8;
    g_info.C[i].component = malloc(w*h);
    g_info.C[i].width = w;
    g_info.C[i].height = h;
  }

  for (row=0; row < mcuy; row++) {
    for (col=0; col < mcux; col++) {
      decode_mcu(col, row, sofn, sos, dht, dht_size, dqt, dqt_size);
    }
  }

  for (i=0; i<sofn->Nf; i++) {
    print_component(i);
    free(g_info.C[i].component);
  }
}

static int decode(struct dht_t* dht, int* code, int* size)
{
  int i = 0, j = 0, value = 0;
  int _code = 0;

  for (i=0; i<16; i++) {
    _code = (_code << 1) + nextbit();
    if (_code <= dht->MAXCODE[i]) {
      break;
    }
  }

  j = dht->VALPTR[i];
  j = j + _code - dht->MINCODE[i];
  value = dht->V[j]; /* V = HUFFVAL */
  if (code != NULL)
    *code = dht->HUFFCODE[j];
  if (size != NULL)
    *size = dht->HUFFSIZE[j];

  return value;
}

static int nextbit()
{
  static int cnt = 0;
  static uint8_t b = 0;
  int bit = 0;

  if (cnt == 0) {
    b = nextbyte();
    cnt = 8;

    if (b == 0xFF) {
      nextbyte(); /* skip zero byte */
    }
  }

  bit = b >> 7;
  cnt = cnt - 1;
  b = b << 1;

  return bit;
}

static int receive(int ssss)
{
  int i = 0, v = 0;

  for (i=0; i<ssss; i++) {
    v = (v << 1) + nextbit();
  }

  return v;
}

static int extend(int V, int T)
{
  int Vt = (1 << (T-1));

  if (V < Vt) {
    Vt = (-1 << T) + 1;
    V = V + Vt;
  }

  return V;
}

int main(int argc, char* argv[])
{
  struct dqt_t dqt[2];
  struct sofn_t sofn;
  struct dht_t dht[4];
  struct sos_t sos;

  if (argc != 2) {
    printf("option error %d\n", argc);
    return -1;
  }

  g_fp = fopen(argv[1], "rb");
  if (g_fp == NULL) {
    printf("error fopen %s\n", argv[1]);
    return -1;
  }

  /* quantization_table */
  parse_quantization_table(&dqt[0]); print_quantization_table(&dqt[0]);
  parse_quantization_table(&dqt[1]); print_quantization_table(&dqt[1]);

  /* frame_header */
  parse_frame_header(&sofn); print_frame_header(&sofn);

  /* huffman_table */
  parse_huffman_table(&dht[0]); print_huffman_table(&dht[0]);
  parse_huffman_table(&dht[1]); print_huffman_table(&dht[1]);
  parse_huffman_table(&dht[2]); print_huffman_table(&dht[2]);
  parse_huffman_table(&dht[3]); print_huffman_table(&dht[3]);

  /* scan_header */
  parse_scan_header(&sos); print_scan_header(&sos);

  /* entropy-coded segment */
  decode_entropy_coded_segment(&sofn, &sos, dht, 4, dqt, 2);

  fclose(g_fp);

  return 0;
}
