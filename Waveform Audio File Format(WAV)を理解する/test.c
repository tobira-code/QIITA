#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct wav_header {
  uint8_t ChunkID[4];        // "RIFF"
  uint32_t ChunkSize;        // 4 + (8 + SubChunkFmtSize(16)) + (8 + SubChunkDataSize)
  uint8_t FormType[4];       // "WAVE"
  uint8_t SubChunkFmtID[4];  // "fmt "
  uint32_t SubChunkFmtSize;  // 16
#define LPCM (1)
  uint16_t FormatTag;        // 1:LPCM
#define MONO   (1)
#define STEREO (2)
  uint16_t Channels;
#define RATE48KHZ (48000)
  uint32_t SamplesPerSec;
  uint32_t AvgBytesPerSec;   // SamplesPerSec * Channels * BitsPerSample / 8
  uint16_t BlockAlign;       // Channels * BitsPerSample / 8
#define BPS16BIT (16)
  uint16_t BitsPerSample;    // 16
  uint8_t SubChunkDataID[4]; // "data "
  uint32_t SubChunkDataSize; // NumSamples * Channels * BitsPerSample / 8
} __attribute__((packed));

struct wav_header wav_template = {
  "RIFF",                    // uint8_t ChunkID[4];
  0,                         // uint32_t ChunkSize;
  "WAVE",                    // uint8_t FormType[4];
  "fmt ",                    // uint8_t SubChunkFmtID[4];
  16,                        // uint32_t SubChunkFmtSize;
  LPCM,                      // uint16_t FormatTag;
  MONO,                      // uint16_t Channels;
  RATE48KHZ,                 // uint32_t SamplesPerSec;
  RATE48KHZ*MONO*BPS16BIT/8, // uint32_t AvgBytesPerSec;
  MONO*BPS16BIT/8,           // uint16_t BlockAlign;
  BPS16BIT,                  // uint16_t BitsPerSample;
  "data",                    // uint8_t SubChunkDataID[4];
  0,                         // uint32_t SubChunkDataSize;
};

double chirp(int x, int N, double sf /*sampling_frequency*/, double fs, double fe, double A /*amplitude*/)
{
  double k = (fe - fs) / (double)N;
  return A * sin(2.0 * M_PI / sf * (fs*(double)x + k/2.0 * (double)x * (double)x));
}

/* (write size) = N * sizeof(int16_t) */
void create_and_write_data(FILE* fp, int N, double fs /* start frequency*/, double fe /* end frequency*/, double A /*amplitude*/)
{
  int i;
  size_t ret;
  
  for (i=0; i<N; i++) {
    int16_t s = (int16_t)round(chirp(i, N, (double)RATE48KHZ, fs, fe, A));
    ret = fwrite(&s, 1, sizeof(s), fp);
    if (ret != sizeof(s)) {
      printf("error fwrite %d\n", __LINE__);
      return;
    }
  }
}

void usage()
{
  printf("usage:\n");
  printf(" test [fs] [fe] [A] [N]\n");
}

int main(int argc, char* argv[])
{
  int fs, fe, A, N;
  size_t ret;
  FILE* fp;
  
  if (argc != 5) {
    usage();
    return -1;
  }
  
  fs = atoi(argv[1]);
  fe = atoi(argv[2]);
  A = atoi(argv[3]);
  N = atoi(argv[4]);
  
  printf("fs=%d fe=%d A=%d N=%d\n", fs, fe, A, N);
  
  fp = fopen("test.wav", "wb");
  if (fp == NULL) {
    printf("error fopen\n");
    return -1;
  }
  
  wav_template.SubChunkDataSize = N * sizeof(int16_t);
  wav_template.ChunkSize = 36 + wav_template.SubChunkDataSize;
  
  // header
  ret = fwrite(&wav_template, 1, sizeof(wav_template), fp);
  if (ret != sizeof(wav_template)) {
    printf("error fwrite %d\n", __LINE__);
    return -1;
  }
  
  // data
  create_and_write_data(fp, N, (double)fs, (double)fe, (double)A);
  
  fclose(fp);
  return 0;
}