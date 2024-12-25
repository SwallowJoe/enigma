# enigma

A little gui lib based on Vulkan.

## Architecture

Mock of Skia to implement a 2D render engine.

### Dependencies

- Vulkan
- glfw
- libpng
- libjpeg
- libwebp
- libtiff
- libheif

### Features

- [x] 2D Render
- [x] Image Decode
- [x] Image Encode
- [x] GLSL Shader Generator

## Build

使用根目录下的build.sh脚本即可。

## Sample

绘制200x200大小并用图片填充的矩形示例:

```c++
#include "include/core/EgCanvas.h"
#include "include/core/EgPaint.h"
#include "include/core/EgRect.h"

int main() {
    // 创建一个EgBitmap对象，这将作为我们的画布
    EgBitmap bitmap;
    bitmap.allocN32Pixels(200, 200); // 分配一个200x200像素的画布

    EgCanvas canvas(bitmap);

    // 设置抗锯齿
    EgPaint paint;
    paint.setAntiAlias(true);

    // 设置颜色
    paint.setColor(EG_ColorRED);

    // 设置矩形的边界
    EgRect rect = EgRect::MakeXYWH(50, 50, 100, 100);

    // 绘制矩形
    canvas.drawRect(rect, paint);

    // 保存绘制的位图到文件
    EgPixmap pixmap;
    bitmap.peekPixels(&pixmap);
    EgImageEncoder::EncodeFile("rectangle.png", pixmap, EgImageEncoder::kPNG_Type, 100);

    return 0;
}
```