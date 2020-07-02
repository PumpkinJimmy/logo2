#pragma once
#ifndef _CANVAS_H
#define _CANVAS_H
#include <utility>
#include <array>
#include <memory>
using std::tuple;
using std::pair;
using std::array;
using std::shared_ptr;
using Point = array<double, 2>;
using Color = array<int, 3>;
/*
 * 绘版基类
 * 提供接口来接入多种绘图前端（如SVG、cairo、Qt）
 */
class Canvas
{
public:
	virtual bool drawLine(const Point& st, const Point& ed, const Color& color) = 0;
};
typedef shared_ptr<Canvas> CanvasPtr;

class DummyCanvas : public Canvas
{
public:
	virtual bool drawLine(const Point& st, const Point& ed, const Color& color) { return true; }

};
#endif