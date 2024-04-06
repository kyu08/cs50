#include "helpers.h"

#include <math.h>
#include <stdint.h>

typedef struct {
  uint16_t sum;   // uint8同士の足し算なのでuint8のままだとoverflowする可能性がある
  uint8_t count;  // 最大9なので8bitあれば十分
} Result;

const uint8_t MAX_8_BIT_NUMBER = 255;

Result get_above_line_result(int height, int width, int current_height, int current_width,
                             RGBTRIPLE image[height][width], BYTE (*func)(RGBTRIPLE));
Result get_current_line_result(int height, int width, int current_height, int current_width,
                               RGBTRIPLE image[height][width], BYTE (*func)(RGBTRIPLE));
Result get_below_line_result(int height, int width, int current_height, int current_width,
                             RGBTRIPLE image[height][width], BYTE (*get_color_amount)(RGBTRIPLE));

BYTE get_blue(RGBTRIPLE rgbtriple);
BYTE get_green(RGBTRIPLE rgbtriple);
BYTE get_red(RGBTRIPLE rgbtriple);

uint16_t egde_result(int height, int width, RGBTRIPLE image[height][width], int h, int w,
                     BYTE (*get_color_amount)(RGBTRIPLE));

long get_above_line_gx(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                       BYTE (*get_color_amount)(RGBTRIPLE));
long get_current_line_gx(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                         BYTE (*get_color_amount)(RGBTRIPLE));
long get_below_line_gx(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                       BYTE (*get_color_amount)(RGBTRIPLE));

long get_above_line_gy(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                       BYTE (*get_color_amount)(RGBTRIPLE));

long get_current_line_gy(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                         BYTE (*get_color_amount)(RGBTRIPLE));

long get_below_line_gy(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                       BYTE (*get_color_amount)(RGBTRIPLE));

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
  // rgbの平均を取り、最も近い整数に丸める
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      RGBTRIPLE *i = &image[h][w];
      BYTE average = round((i->rgbtRed + i->rgbtGreen + i->rgbtBlue) / 3.0);
      i->rgbtRed = average;
      i->rgbtGreen = average;
      i->rgbtBlue = average;
    }
  }

  return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width / 2; w++) {
      // width %2 == 1 のとき真ん中の要素においてw == width - wとなる。この場合はswapは不要なのでcontinueする
      if (w == width - w) {
        continue;
      }

      RGBTRIPLE temp = image[h][w];
      image[h][w] = image[h][width - w - 1];
      image[h][width - w - 1] = temp;
    }
  }
  return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE filtered_image[height][width];
  // 隣接する9マスの平均をとる
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      Result blue_above_r = get_above_line_result(height, width, h, w, image, get_blue);
      Result blue_current_r = get_current_line_result(height, width, h, w, image, get_blue);
      Result blue_below_r = get_below_line_result(height, width, h, w, image, get_blue);
      BYTE average_blue = round((blue_above_r.sum + blue_current_r.sum + blue_below_r.sum) /
                                (float)(blue_above_r.count + blue_current_r.count + blue_below_r.count));
      filtered_image[h][w].rgbtBlue = average_blue;

      Result green_above_r = get_above_line_result(height, width, h, w, image, get_green);
      Result green_current_r = get_current_line_result(height, width, h, w, image, get_green);
      Result green_below_r = get_below_line_result(height, width, h, w, image, get_green);
      BYTE average_green = round((green_above_r.sum + green_current_r.sum + green_below_r.sum) /
                                 (float)(green_above_r.count + green_current_r.count + green_below_r.count));
      filtered_image[h][w].rgbtGreen = average_green;

      Result red_above_r = get_above_line_result(height, width, h, w, image, get_red);
      Result red_current_r = get_current_line_result(height, width, h, w, image, get_red);
      Result red_below_r = get_below_line_result(height, width, h, w, image, get_red);
      BYTE average_red = round((red_above_r.sum + red_current_r.sum + red_below_r.sum) /
                               (float)(red_above_r.count + red_current_r.count + red_below_r.count));
      filtered_image[h][w].rgbtRed = average_red;
    }
  }

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      image[h][w] = filtered_image[h][w];
    }
  }
  return;
}

// get_above_line_result 一つ上の行の合計と要素数を返す
Result get_above_line_result(int height, int width, int current_height, int current_width,
                             RGBTRIPLE image[height][width], BYTE (*get_color_amount)(RGBTRIPLE)) {
  Result r;
  r.sum = 0;
  r.count = 0;

  if (current_height == 0) {  // 一番上の行だった場合
    return r;
  }
  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルの真上とその右が対象
    r.sum += get_color_amount(image[current_height - 1][current_width]);
    r.count++;
    r.sum += get_color_amount(image[current_height - 1][current_width + 1]);
    r.count++;

    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルの真上とその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r.sum += get_color_amount(image[current_height - 1][current_width - 1]);
    r.count++;
    r.sum += get_color_amount(image[current_height - 1][current_width]);
    r.count++;

    return r;
  }

  r.sum += get_color_amount(image[current_height - 1][current_width - 1]);
  r.count++;
  r.sum += get_color_amount(image[current_height - 1][current_width]);
  r.count++;
  r.sum += get_color_amount(image[current_height - 1][current_width + 1]);
  r.count++;

  return r;
};

// get_current_line_result 現在の行の合計と要素数を返す
Result get_current_line_result(int height, int width, int current_height, int current_width,
                               RGBTRIPLE image[height][width], BYTE (*get_color_amount)(RGBTRIPLE)) {
  Result r;
  r.sum = 0;
  r.count = 0;

  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルとその右が対象
    r.sum += get_color_amount(image[current_height][current_width]);
    r.count++;
    r.sum += get_color_amount(image[current_height][current_width + 1]);
    r.count++;

    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルとその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r.sum += get_color_amount(image[current_height][current_width - 1]);
    r.count++;
    r.sum += get_color_amount(image[current_height][current_width]);
    r.count++;

    return r;
  }

  r.sum += get_color_amount(image[current_height][current_width - 1]);
  r.count++;
  r.sum += get_color_amount(image[current_height][current_width]);
  r.count++;
  r.sum += get_color_amount(image[current_height][current_width + 1]);
  r.count++;

  return r;
};

// get_below_line_result 一つ下の行の合計と要素数を返す
Result get_below_line_result(int height, int width, int current_height, int current_width,
                             RGBTRIPLE image[height][width], BYTE (*get_color_amount)(RGBTRIPLE)) {
  Result r;
  r.sum = 0;
  r.count = 0;

  if (current_height == height - 1) {  // 一番下の行だった場合
    return r;
  }
  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルの真下とその右が対象
    r.sum += get_color_amount(image[current_height + 1][current_width]);
    r.count++;
    r.sum += get_color_amount(image[current_height + 1][current_width + 1]);
    r.count++;

    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルの真下とその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r.sum += get_color_amount(image[current_height + 1][current_width - 1]);
    r.count++;
    r.sum += get_color_amount(image[current_height + 1][current_width]);
    r.count++;

    return r;
  }

  r.sum += get_color_amount(image[current_height + 1][current_width - 1]);
  r.count++;
  r.sum += get_color_amount(image[current_height + 1][current_width]);
  r.count++;
  r.sum += get_color_amount(image[current_height + 1][current_width + 1]);
  r.count++;

  return r;
};

BYTE get_blue(RGBTRIPLE rgbtriple) { return rgbtriple.rgbtBlue; }
BYTE get_green(RGBTRIPLE rgbtriple) { return rgbtriple.rgbtGreen; }
BYTE get_red(RGBTRIPLE rgbtriple) { return rgbtriple.rgbtRed; }

// Detect edges
// √(Gx^2 + Gy^2)
void edges(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE filtered_image[height][width];
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      filtered_image[h][w].rgbtBlue = egde_result(height, width, image, h, w, get_blue);
      filtered_image[h][w].rgbtGreen = egde_result(height, width, image, h, w, get_green);
      filtered_image[h][w].rgbtRed = egde_result(height, width, image, h, w, get_red);
    }
  }

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      image[h][w] = filtered_image[h][w];
    }
  }
  return;
}

uint16_t egde_result(int height, int width, RGBTRIPLE image[height][width], int h, int w,
                     BYTE (*get_color_amount)(RGBTRIPLE)) {
  long gx = 0;
  gx += get_above_line_gx(height, width, h, w, image, get_color_amount);
  gx += get_current_line_gx(height, width, h, w, image, get_color_amount);
  gx += get_below_line_gx(height, width, h, w, image, get_color_amount);

  long gy = 0;
  gy += get_above_line_gy(height, width, h, w, image, get_color_amount);
  gy += get_current_line_gy(height, width, h, w, image, get_color_amount);
  gy += get_below_line_gy(height, width, h, w, image, get_color_amount);

  uint16_t not_capped_result = round(sqrt((gx * gx) + (gy * gy)));

  if (MAX_8_BIT_NUMBER < not_capped_result) {
    return MAX_8_BIT_NUMBER;
  }
  return not_capped_result;
}

long get_above_line_gx(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                       BYTE (*get_color_amount)(RGBTRIPLE)) {
  long r = 0;

  if (current_height == 0) {  // 一番上の行だった場合
    return r;
  }
  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルの真上とその右が対象
    // 必要ないけどロジックの可読性のために書いている
    r += get_color_amount(image[current_height - 1][current_width]) * 0;
    r += get_color_amount(image[current_height - 1][current_width + 1]) * 1;
    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルの真上とその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r += get_color_amount(image[current_height - 1][current_width - 1]) * (-1);
    r += get_color_amount(image[current_height - 1][current_width]) * 0;
    return r;
  }

  r += get_color_amount(image[current_height - 1][current_width - 1]) * (-1);
  r += get_color_amount(image[current_height - 1][current_width]) * 0;
  r += get_color_amount(image[current_height - 1][current_width + 1]) * 1;
  return r;
};

long get_current_line_gx(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                         BYTE (*get_color_amount)(RGBTRIPLE)) {
  long r = 0;

  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルとその右が対象
    r += get_color_amount(image[current_height][current_width]) * 0;
    r += get_color_amount(image[current_height][current_width + 1]) * 2;

    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルとその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r += get_color_amount(image[current_height][current_width - 1]) * (-2);
    r += get_color_amount(image[current_height][current_width]) * 0;
    return r;
  }

  r += get_color_amount(image[current_height][current_width - 1]) * (-2);
  r += get_color_amount(image[current_height][current_width]) * 0;
  r += get_color_amount(image[current_height][current_width + 1]) * 2;
  return r;
};

long get_below_line_gx(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                       BYTE (*get_color_amount)(RGBTRIPLE)) {
  long r = 0;

  if (current_height == height - 1) {  // 一番下の行だった場合
    return r;
  }
  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルの真下とその右が対象
    r += get_color_amount(image[current_height + 1][current_width]) * 0;
    r += get_color_amount(image[current_height + 1][current_width + 1]) * 1;

    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルの真下とその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r += get_color_amount(image[current_height + 1][current_width - 1]) * (-1);
    r += get_color_amount(image[current_height + 1][current_width]) * 0;

    return r;
  }

  r += get_color_amount(image[current_height + 1][current_width - 1]) * (-1);
  r += get_color_amount(image[current_height + 1][current_width]) * 0;
  r += get_color_amount(image[current_height + 1][current_width + 1]) * 1;
  return r;
};

long get_above_line_gy(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                       BYTE (*get_color_amount)(RGBTRIPLE)) {
  long r = 0;

  if (current_height == 0) {  // 一番上の行だった場合
    return r;
  }
  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルの真上とその右が対象
    // 必要ないけどロジックの可読性のために書いている
    r += get_color_amount(image[current_height - 1][current_width]) * (-2);
    r += get_color_amount(image[current_height - 1][current_width + 1]) * (-1);
    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルの真上とその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r += get_color_amount(image[current_height - 1][current_width - 1]) * (-1);
    r += get_color_amount(image[current_height - 1][current_width]) * (-2);
    return r;
  }

  r += get_color_amount(image[current_height - 1][current_width - 1]) * (-1);
  r += get_color_amount(image[current_height - 1][current_width]) * (-2);
  r += get_color_amount(image[current_height - 1][current_width + 1]) * (-1);
  return r;
};

long get_current_line_gy(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                         BYTE (*get_color_amount)(RGBTRIPLE)) {
  long r = 0;

  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルとその右が対象
    r += get_color_amount(image[current_height][current_width]) * 0;
    r += get_color_amount(image[current_height][current_width + 1]) * 0;

    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルとその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r += get_color_amount(image[current_height][current_width - 1]) * 0;
    r += get_color_amount(image[current_height][current_width]) * 0;
    return r;
  }

  r += get_color_amount(image[current_height][current_width - 1]) * 0;
  r += get_color_amount(image[current_height][current_width]) * 0;
  r += get_color_amount(image[current_height][current_width + 1]) * 0;
  return r;
};
long get_below_line_gy(int height, int width, int current_height, int current_width, RGBTRIPLE image[height][width],
                       BYTE (*get_color_amount)(RGBTRIPLE)) {
  long r = 0;

  if (current_height == height - 1) {  // 一番下の行だった場合
    return r;
  }
  if (current_width == 0) {  // 一番左の列だった場合は中央のピクセルの真下とその右が対象
    r += get_color_amount(image[current_height + 1][current_width]) * 2;
    r += get_color_amount(image[current_height + 1][current_width + 1]) * 1;

    return r;
  }
  if (current_width == width - 1) {  // 一番右の列だった場合は中央のピクセルの真下とその左が対象
                                     // current_widthは0始まりだがwidthは1始まりなので-1している。
    r += get_color_amount(image[current_height + 1][current_width - 1]) * 1;
    r += get_color_amount(image[current_height + 1][current_width]) * 2;

    return r;
  }

  r += get_color_amount(image[current_height + 1][current_width - 1]) * 1;
  r += get_color_amount(image[current_height + 1][current_width]) * 2;
  r += get_color_amount(image[current_height + 1][current_width + 1]) * 1;
  return r;
};
