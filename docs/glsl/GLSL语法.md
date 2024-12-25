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