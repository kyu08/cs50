#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: ./recover [file_name]\n");
    return 1;
  }

  FILE *input = fopen(argv[1], "rb");
  if (input == NULL) {
    printf("File opening failed\n");
    return 1;
  }

  const uint16_t FILE_BLOCK_UNIT = 512;
  unsigned char buffer[FILE_BLOCK_UNIT];
  FILE *output = NULL;  // 出力ファイルポインタを初期化
  char file_name[8];
  int file_count = 0;  // 見つかったJPEGファイルの数を数える

  while (fread(buffer, sizeof(char), FILE_BLOCK_UNIT, input) == FILE_BLOCK_UNIT) {
    // JPEGヘッダであるかを確認
      if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
      // 既存のファイルがあれば閉じる
      if (output != NULL) {
        fclose(output);
      }
      // 新しいファイルを開く
      sprintf(file_name, "%03i.jpg", file_count++);
      output = fopen(file_name, "w");
      if (output == NULL) {
        printf("File opening failed\n");
        return 1;
      }
    }
    // 開いている出力ファイルにbufferを書き込む
    if (output != NULL) {
      fwrite(buffer, sizeof(char), FILE_BLOCK_UNIT, output);
    }
  }

  // 後始末
  if (output != NULL) {
    fclose(output);
  }
  fclose(input);

  return 0;
}
