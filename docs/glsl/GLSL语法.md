# GLSL语法

## GLSL 简介

GLSL（OpenGL Shading Language）是一种用于编写图形着色器的高级语言。它是 OpenGL 标准的一部分，用于在图形卡上执行图形渲染。GLSL 提供了丰富的数学和逻辑运算功能，使得开发者可以编写复杂的着色器程序。

GLSL 基于 C 语言的扩展，它提供了丰富的数学和逻辑运算功能。GLSL 支持向量和矩阵运算，以及各种数学函数。GLSL 还支持纹理采样和立方体贴图采样。

GLSL 主要用于定义顶点着色器（Vertex Shaders）和片元着色器（Fragment Shaders），以及几何着色器（Geometry Shaders）、曲面着色器（Tessellation Shaders）和计算着色器（Compute Shaders），这些着色器共同用于渲染管线的不同阶段。

## GLSL 版本

GLSL 有两个版本：GLSL 1.0 和 GLSL 1.1。GLSL 1.0 是最早的版本，它提供了基本的数学和逻辑运算功能。GLSL 1.1 是 GLSL 1.0 的扩展，它提供了更多的数学和逻辑运算功能。

### GLSL 版本声明

在 GLSL 程序中，需要使用 `#version` 指令来声明 GLSL 版本。

```glsl

#version 330 core
```
### GLSL 版本兼容性

GLSL 1.0 和 GLSL 1.1 之间存在一些兼容性问题。GLSL 1.1 引入了一些新的特性，但是旧的 GLSL 1.0 程序可能无法使用这些特性。


## 数据类型

GLSL 支持以下数据类型：
- `int`：整数类型。
- `float`：浮点数类型。
- `vec2`：包含两个浮点数的向量类型。
- `vec3`：包含三个浮点数的向量类型。
- `vec4`：包含四个浮点数的向量类型。
- `mat2`：包含两个浮点数的矩阵类型。
- `mat3`：包含三个浮点数的矩阵类型。
- `mat4`：包含四个浮点数的矩阵类型。
- `sampler2D`：用于纹理采样的类型。
- `samplerCube`：用于立方体贴图采样的类型。


## 变量声明

在 GLSL 中，变量需要在使用前进行声明。变量可以是全局变量或局部变量。

```glsl
int a; // 全局变量

void main() {
    float b; // 局部变量
}
```

## 常量

GLSL 支持常量，常量可以是整数、浮点数或向量。常量可以在程序中多次使用。

```glsl
const float PI = 3.1415926;

void main() {
    float radius = 10.0;
    float area = PI * radius * radius;
}
```

## 函数

在 GLSL 中，函数可以接受参数并返回值。函数可以在程序中多次使用。

```glsl
float add(float a, float b) {
    return a + b;
}
```

## 控制流

在 GLSL 中，支持以下控制流语句：

- `if`：条件语句。
- `for`：循环语句。
- `while`：循环语句。

```glsl
if (a > b) {
    // do something
}

for (int i = 0; i < 10; i++) {
    // do something
}

while (a < b) {
    // do something
}
```

## 数组

在 GLSL 中，数组可以存储多个相同类型的值。数组可以是一维数组或多维数组。

```glsl
float vertices[3] = vec3(0.0, 0.0, 0.0);
float matrix[3][3] = mat3(1.0);
```

## 结构体

在 GLSL 中，结构体可以存储多个不同类型的值。结构体可以在程序中多次使用。

```glsl
struct Vertex {
    vec3 position;
    vec3 normal;
};

Vertex vertex;
```

## 内置变量

GLSL 提供了一些内置变量，这些变量可以在着色器程序中使用。例如，`gl_Position` 是顶点着色器中的内置变量，用于设置顶点的位置。

```glsl
void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
```

在 GLSL 中，有一些内置变量可以用于访问渲染管线的状态和数据。

- `gl_Position`：顶点着色器输出的顶点位置。
- `gl_FragCoord`：片元着色器输入的片元位置。
- `gl_FragColor`：片元着色器输出的片元颜色。
- `gl_ModelViewMatrix`：模型视图矩阵。
- `gl_ProjectionMatrix`：投影矩阵。
- `gl_NormalMatrix`：法线矩阵。

## 内置函数

GLSL 提供了一些内置函数，这些函数可以在着色器程序中使用。例如，`sin` 函数用于计算正弦值。

```glsl
float angle = 3.1415926 / 2.0;
float result = sin(angle);
```

在 GLSL 中，有一些内置函数可以用于执行常见的数学和逻辑运算。

- `abs()`：返回绝对值。
- `sin()`：返回正弦值。
- `cos()`：返回余弦值。
- `tan()`：返回正切值。
- `dot()`：返回两个向量的点积。
- `cross()`：返回两个向量的叉积。

## 着色器程序

在 GLSL 中，着色器程序由顶点着色器、片元着色器和可选的几何着色器、曲面着色器和计算着色器组成。每个着色器负责不同的渲染阶段。

```glsl
#version 330 core

layout(location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0);
}
```
