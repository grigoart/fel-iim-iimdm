#include "iimavlib/SDLDevice.h"
#ifdef SYSTEM_LINUX
#include <unistd.h>
#endif
#include <algorithm>
#include <vector>
#include "iimavlib/Utils.h"
#include "iimavlib/video_ops.h"
#include <stdlib.h>     /* srand, rand */
#include <sys/timeb.h>
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string

using std::unique_ptr;

using namespace iimavlib;

class Context;
class App;

int A[] = {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1};
int B[] = {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0};
int C[] = {0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1};
int D[] = {1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0};
int E[] = {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1};
int F[] = {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0};
int G[] = {0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1};
int H[] = {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1};
int I[] = {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1};
int J[] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1};
int K[] = {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1};
int L[] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1};
int M[] = {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1};
int N[] = {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1};
int O[] = {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1};
int P[] = {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0};
int Q[] = {1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1};
int R[] = {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1};
int S[] = {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0};
int T[] = {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};
int U[] = {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1};
int V[] = {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0};
int W[] = {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
int X[] = {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1};
int Y[] = {1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0};
int Z[] = {1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1};
int N0[] = {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1};
int N1[] = {0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1};
int N2[] = {1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1};
int N3[] = {1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1};
int N4[] = {1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1};
int N5[] = {1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0};
int N6[] = {0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1};
int N7[] = {1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0};
int N8[] = {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1};
int N9[] = {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0};
int SP[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int MINUS[] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0};
int PLUS[] = {0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0};
int COLON[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};

int WIN_W = 1024;
int WIN_H = 768;

int SIZE_LINE_BOLD = 3;
int SIZE_LINE_THIN = 1;

rgb_t COLOR_BG = rgb_t(25, 25, 25);
rgb_t COLOR_LINE = rgb_t(255, 0, 0);

bool isIn(int x, int y, rectangle_t rect) {
	return x > rect.x && x < rect.x + rect.width && y > rect.y && y < rect.y + rect.height;
}

class Drawable {
	public:
		virtual void draw(Context& ctx) = 0;
};

class Updatable {
	public:
		virtual void update(int delta) = 0;
};

class Clickable {
	public:
		virtual rectangle_t hitrectangle() = 0;
		virtual void click(int x, int y, int b) = 0;
};

class Context {
	public:
		SDLDevice* sdl;
		video_buffer_t* data;
		int width, height;
		Context(SDLDevice& sdl_c, video_buffer_t& data_c) {
			sdl = &sdl_c;
			data = &data_c;
			width = data_c.size.width;
			height = data_c.size.height;
		}
		void draw(Drawable& drawable) {
			drawable.draw(*this);
		}
		int render() {
			return sdl->blit(*data);
		}
void clear(rgb_t color) {data->clear(color);}
void pixel(int x, int y, rgb_t color) {(*data)(x, y) = color;}
void line(int x0, int y0, int x1, int y1, rgb_t color) {draw_line(*data, rectangle_t(x0,y0), rectangle_t(x1,y1), color);}
void circle(int x, int y, int r, rgb_t color) {circle(x, y, 2 * r, 2 * r, color);}
void circle(int x, int y, int width, int height, rgb_t color) {draw_circle(*data, rectangle_t(x, y, x + width, y + height), color);}
void rectangle(int x, int y, int width, int height, rgb_t color) {draw_rectangle(*data, rectangle_t(x, y, width, height), color);}
void emptyRectangle(int x, int y, int width, int height, int border, rgb_t color) {draw_empty_rectangle(*data, rectangle_t(x, y, width, height), border, color);}
void emptyRectangle(int x, int y, int width, int height, rgb_t color) {emptyRectangle(x, y, width, height, 1, color);}
void polyline(const std::vector<rectangle_t>& points, rgb_t color) {draw_polyline(*data, points, color);}
void polygon(const std::vector<rectangle_t>& points, rgb_t color) {draw_polyline(*data, points, color);}
void triangle(int x1, int y1, int x2, int y2, int x3, int y3, rgb_t color) {
	std::vector<rectangle_t> v;
	v.push_back(rectangle_t(x1, y1, 0, 0));
	v.push_back(rectangle_t(x2, y2, 0, 0));
	v.push_back(rectangle_t(x3, y3, 0, 0));
	v.push_back(rectangle_t(x1, y1, 0, 0));
	polygon(v, color);
};
//void draw_polyline(iimavlib::video_buffer_t& data, const std::vector<iimavlib::rectangle_t>& points, iimavlib::rgb_t color)
//void draw_polygon(iimavlib::video_buffer_t& data, const std::vector<iimavlib::rectangle_t>& points, iimavlib::rgb_t color)
//void draw_line_steep(iimavlib::video_buffer_t& data, iimavlib::rectangle_t start, iimavlib::rectangle_t end, iimavlib::rgb_t color)
//void draw_line_thick(iimavlib::video_buffer_t& data, iimavlib::rectangle_t start, iimavlib::rectangle_t end, int border, iimavlib::rgb_t color)
};

class Text: Drawable {
	public:
		int x, y;
		int size = 2;
		std::string* strRef;
		std::string strStatic;
		Text(int xc, int yc, std::string* strRefc) {
			x = xc;
			y = yc;
			strRef = strRefc;
		}
		Text(int xc, int yc, std::string strStaticc) {
			x = xc;
			y = yc;
			strStatic = strStaticc;
			strRef = &strStatic;
		}
		size_t length() {
			return strRef->length();
		}
		void draw(Context& ctx) {
			for (size_t i = 0; i < strRef->length(); i++) {
				int* arr = A;
				switch ((*strRef).at(i)) {
					case 'a': case 'A': arr = A; break;
					case 'b': case 'B': arr = B; break;
					case 'c': case 'C': arr = C; break;
					case 'd': case 'D': arr = D; break;
					case 'e': case 'E': arr = E; break;
					case 'f': case 'F': arr = F; break;
					case 'g': case 'G': arr = G; break;
					case 'h': case 'H': arr = H; break;
					case 'i': case 'I': arr = I; break;
					case 'j': case 'J': arr = J; break;
					case 'k': case 'K': arr = K; break;
					case 'l': case 'L': arr = L; break;
					case 'm': case 'M': arr = M; break;
					case 'n': case 'N': arr = N; break;
					case 'o': case 'O': arr = O; break;
					case 'p': case 'P': arr = P; break;
					case 'q': case 'Q': arr = Q; break;
					case 'r': case 'R': arr = R; break;
					case 's': case 'S': arr = S; break;
					case 't': case 'T': arr = T; break;
					case 'u': case 'U': arr = U; break;
					case 'v': case 'V': arr = V; break;
					case 'w': case 'W': arr = W; break;
					case 'x': case 'X': arr = X; break;
					case 'y': case 'Y': arr = Y; break;
					case 'z': case 'Z': arr = Z; break;
					case '0': arr = N0; break;
					case '1': arr = N1; break;
					case '2': arr = N2; break;
					case '3': arr = N3; break;
					case '4': arr = N4; break;
					case '5': arr = N5; break;
					case '6': arr = N6; break;
					case '7': arr = N7; break;
					case '8': arr = N8; break;
					case '9': arr = N9; break;
					case ' ': arr = SP; break;
					case '-': arr = MINUS; break;
					case '+': arr = PLUS; break;
					case ':': arr = COLON; break;
					default: break;
				}
				for (size_t j = 0; j < 15; j++) {
				 if (arr[j]) {
						ctx.rectangle(x + 4*size * i + (j % 3)*size, y + (j / 3)*size, size, size, rgb_t(255, 255, 255));
					}
				}
			}
		}
};

class CenteredText: public Drawable {
	public:
		int x, y, width, height;
		Text* text;
		CenteredText(int xc, int yc, int widthc, int heightc, std::string* strRefc) {
			x = xc;
			y = yc;
			width = widthc;
			height = heightc;
			text = new Text(x, y, strRefc);
		}
		CenteredText(int xc, int yc, int widthc, int heightc, std::string strStaticc) {
			x = xc;
			y = yc;
			width = widthc;
			height = heightc;
			text = new Text(x, y, strStaticc);
		}
		void draw(Context& ctx) {
			int textLen = text->size * 3 * text->length() + (text->length() - 1) * text->size;
			text->x = x + (width - textLen) / 2;
			text->y = y + (height - text->size * 5) / 2;
			text->draw(ctx);
		}
};

class Button: public Drawable {
	public:
		int x, y, width, height;
		rgb_t color;
		bool active = false;
		bool on = false;
		Button(int xc, int yc, int widthc, int heightc, rgb_t colorc) {
			x = xc;
			y = yc;
			width = widthc;
			height = heightc;
			color = colorc;
			on = (rand() % 2);
		}
		void draw(Context& ctx) {
			if (active) {
				ctx.rectangle(x, y, width, height, rgb_t(color.r / 2, color.g / 2, color.b / 2));
			}
			ctx.emptyRectangle(x, y, width, height, (on) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, color);
		}
		rectangle_t hitrectangle() {
			return rectangle_t(x, y, width, height);
		}
		void click(int, int, int) {
			on = !on;
		}
		void activate() {
			if (on) active = true;
		}
		void deactivate() {
			active = false;
		}
		void toggle() {
			if (active) deactivate(); else activate();
		}
};

class HeadControlButton: public Drawable, public Clickable {
	public:
		int x, y;
		static const int height = 14;
		static const int width = 14;
		rgb_t bColor = rgb_t(0, 200, 0);
		rgb_t bColorAlt = rgb_t(0, 100, 0);
		bool on = true;
		HeadControlButton(int xc, int yc) {
			x = xc;
			y = yc;
		}
		void draw(Context& ctx) {
			ctx.rectangle(x, y, width, height, (on) ? bColor : bColorAlt);
			ctx.emptyRectangle(x, y, width, height, SIZE_LINE_BOLD, rgb_t(0, 255, 0));
		}
		rectangle_t hitrectangle() {
			return rectangle_t(x, y, width, height);
		}
		void click(int, int, int) {
			on = !on;
		}
};

class Head: public Drawable {
	public:
		int x, y, width, height;
		rgb_t color;
		bool on = true;
		Text text;
		HeadControlButton* muteB;
		Head(int xc, int yc, int widthc, int heightc, std::string& caption, rgb_t colorc):
			text(xc + 8, yc + 10, &caption) {
			x = xc;
			y = yc;
			width = widthc;
			height = heightc;
			color = colorc;
			muteB = new HeadControlButton(x + width - muteB->width - 8, y + height - muteB->height - 8);
		}
		void draw(Context& ctx) {
			ctx.emptyRectangle(x, y, width, height, (on) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, color);
			text.draw(ctx);
			muteB->draw(ctx);
		}
		rectangle_t hitrectangle() {
			return rectangle_t(x, y, width, height);
		}
		void click(int x, int y, int) {
			if (isIn(x, y, muteB->hitrectangle())) {
				muteB->click(x, y, 0);
			}
		}
};

class ButtonCol: public Drawable, public Clickable {
	public:
		int x, y;
		size_t count;
		int bSize = 30;
		int bGap = 5;
		rgb_t bColorAlt = rgb_t(175, 0, 0);
		std::vector<Button> b;
		ButtonCol(int xc, int yc, int countc) {
			x = xc;
			y = yc;
			count = countc;
			for (size_t i = 0; i < count; i++) {
				b.push_back(Button(x, y + (bSize + bGap) * i, bSize, bSize, COLOR_LINE));
			}
		}
		void draw(Context& ctx) {
			for (size_t i = 0; i < count; i++) {
				b.at(i).draw(ctx);
			}
		}
		rectangle_t hitrectangle() {
			return rectangle_t(x, y, bSize, (bSize + bGap) * count);
		}
		void click(int x, int y, int) {
			for (size_t j = 0; j < count; j++) {
				if (isIn(x, y, b.at(j).hitrectangle())) {
					b.at(j).click(x, y, 0);
				}
			}
		}
		void activate() {
			for (size_t i = 0; i < count; i++) {
				b.at(i).activate();
			}
		}
		void deactivate() {
			for (size_t i = 0; i < count; i++) {
				b.at(i).deactivate();
			}
		}
};

class HeadCol: public Drawable, public Clickable {
	public:
		int x, y;
		size_t count;
		int bHeight = 30;
		int bWidth = 125;
		int bGap = 5;
		rgb_t bColorAlt = rgb_t(175, 0, 0);
		std::vector<Head> b;
		HeadCol(int xc, int yc, std::vector<std::string>& captions) {
			x = xc;
			y = yc;
			count = captions.size();
			for (size_t i = 0; i < count; i++) {
				b.push_back(Head(x, y + (bHeight + bGap) * i, bWidth, bHeight, captions.at(i), COLOR_LINE));
			}
		}
		void draw(Context& ctx) {
			for (size_t i = 0; i < count; i++) {
				b.at(i).draw(ctx);
			}
		}
		rectangle_t hitrectangle() {
			return rectangle_t(x, y, bWidth, (bHeight + bGap) * count);
		}
		void click(int x, int y, int) {
			for (size_t j = 0; j < count; j++) {
				if (isIn(x, y, b.at(j).hitrectangle())) {
					b.at(j).click(x, y, 0);
				}
			}
		}
};

class ButtonGrid: public Drawable, public Updatable, public Clickable {
	public:
		int x, y;
		int countX, countY;
		HeadCol* h;
		std::vector<ButtonCol> v;
		int active = -1;
		bool running = false;
		int passedTime = 0;
		int bpm = 120;
		ButtonGrid(int xc, int yc, int countXc, std::vector<std::string>& captions) {
			x = xc;
			y = yc;
			countX = countXc;
			countY = captions.size();
			h = new HeadCol(x, y, captions);
			for (int j = 0; j < countX; j++) {
				v.push_back(ButtonCol(x + h->bGap + h->bWidth + j * 35, y, countY));
			}
		}
		void draw(Context& ctx) {
			h->draw(ctx);
			for (int i = 0; i < countX; i++) {
				v.at(i).draw(ctx);
			}
		}
		void update(int delta) {
			passedTime += delta;
			if (passedTime >= 60000/bpm/4) {
				passedTime -= 60000/bpm/4;
				if (running) activateNext();
			}
		}
		rectangle_t hitrectangle() {return rectangle_t(0, 0, WIN_W, WIN_H);}
		void click(int x, int y, int) {
			if (isIn(x, y, h->hitrectangle())) {
				h->click(x, y, 0);
			}
			for (int j = 0; j < countX; j++) {
				if (isIn(x, y, v.at(j).hitrectangle())) {
					v.at(j).click(x, y, 0);
				}
			}
		}
		void activateNext() {
			if (active == countX - 1) {
				activate(0);
			} else {
				activate(active + 1);
			}
		}
		void activate(int index) {
			if (active != -1) {
				v.at(active).deactivate();
			}
			v.at(index).activate();
			active = index;
		}
		void deactivate() {
			if (active != -1) {
				v.at(active).deactivate();
				active = -1;
			}
		}
		void stop() {
			deactivate();
			active = -1;
			running = false;
		};
		void run() {
			running = true;
		};
		~ButtonGrid() {
			delete h;
		}
};

class PlayButton: public Drawable, public Clickable {
	public:
		int x, y, width;
		rgb_t color;
		bool on = false;
		ButtonGrid* bg;
		PlayButton(int xc, int yc, ButtonGrid* bgc/*, int widthc, int heightc, rgb_t colorc*/) {
			x = xc;
			y = yc;
			width = 55;
			color = COLOR_LINE;
			bg = bgc;
		}
		void draw(Context& ctx) {
			ctx.rectangle(x, y, width, width, COLOR_BG);
			ctx.emptyRectangle(x, y, width, width, SIZE_LINE_BOLD, color);
			int iconWidth = width / 3;
			int leftTopX = x + width / 2 - (iconWidth / 2);
			int leftTopY = y + width / 2 - (iconWidth / 2);
			if (on) {
				ctx.emptyRectangle(leftTopX, leftTopY, iconWidth, iconWidth, COLOR_LINE);
			}
			else {
				ctx.triangle(leftTopX, leftTopY, leftTopX, leftTopY + iconWidth, leftTopX + iconWidth, leftTopY + iconWidth / 2, COLOR_LINE);
			}
		}
		rectangle_t hitrectangle() {
			return rectangle_t(x, y, width, width);
		}
		void click(int, int, int) {
			on = !on;
			if (!on) {
				bg->stop();
			}
			else {
				bg->run();
			}
		}
};

class ValueSpinBtn: public Drawable {
	public:
		int x, y, width, height;
		rgb_t color;
		bool up;
		CenteredText* icon;
		ValueSpinBtn(int xc, int yc, int widthc, int heightc, bool upc) {
			x = xc;
			y = yc;
			width = widthc;
			height = heightc;
			up = upc;
			icon = new CenteredText(x, y, width, height, (up) ? "+" : "-");
		}
		void draw(Context& ctx) {
			ctx.emptyRectangle(x, y, width, height, SIZE_LINE_THIN, COLOR_LINE);
			icon->draw(ctx);
		}
		rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
};

class ValueSpin: public Drawable, public Updatable, public Clickable {
	public:
		int x, y, width = 75, height = 75;
		rgb_t color;
		int* valueRef;
		std::string value;
		int step = 1;
		int bigStep = 5;
		int maxValue = 300;
		int minValue = 1;
		
		CenteredText* caption;
		ValueSpinBtn* valueSpinBtnUp;
		ValueSpinBtn* valueSpinBtnDown;
		CenteredText* valueText;
		ValueSpin(int xc, int yc, int* valueRefc) {
			x = xc;
			y = yc;
			valueRef = valueRefc;
			value = std::to_string(*valueRef);
			caption = new CenteredText(x, y, width, height / 4, "Tempo");
			valueText = new CenteredText(x, y + 2 * height / 4, width, height / 4, &value);
			valueSpinBtnUp = new ValueSpinBtn(x, y + height / 4, width, height / 4, true);
			valueSpinBtnDown = new ValueSpinBtn(x, y + 3 * height / 4, width, height / 4, false);
		}
		void draw(Context& ctx) {
			caption->draw(ctx);
			valueSpinBtnUp->draw(ctx);
			valueText->draw(ctx);
			valueSpinBtnDown->draw(ctx);
		}
		void update(int) {
			value = std::to_string(*valueRef);
		}
		rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
		void click(int x, int y, int b) {
			if (isIn(x, y, valueSpinBtnUp->hitrectangle())) {
				if (b == 0) {
					(*valueRef) += step;
				}
				else {
					(*valueRef) += bigStep;
				}
				if (*valueRef > maxValue) (*valueRef) = maxValue;
			}
			else if (isIn(x, y, valueSpinBtnDown->hitrectangle())) {
				if (b == 0) {
					(*valueRef) -= step;
				}
				else {
					(*valueRef) -= bigStep;
				}
				if (*valueRef < minValue) (*valueRef) = minValue;
			}
		}
};

class ControlPanel: public Drawable, public Updatable, public Clickable {
	public:
		int x, y, width, height;
		rgb_t color;
		PlayButton* play;
		ValueSpin* valueSpin;
		ControlPanel(int xc, int yc, int widthc, int heightc, ButtonGrid* bg/*, int* bpmRef*/) {
			x = xc;
			y = yc;
			width = widthc;
			height = heightc;
			play = new PlayButton(x + 20, y + 20, bg);
			valueSpin = new ValueSpin(x + 200, y + 10, &(bg->bpm));
		}
		void draw(Context& ctx) {
			ctx.emptyRectangle(x, y, width, height, SIZE_LINE_THIN, COLOR_LINE);
			ctx.draw(*play);
			ctx.draw(*valueSpin);
		}
		void update(int delta) {
			valueSpin->update(delta);
		}
		rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
		void click(int x, int y, int b) {
			if (isIn(x, y, play->hitrectangle())) {
				play->click(x, y, 0);
			}
			if (isIn(x, y, valueSpin->hitrectangle())) {
				valueSpin->click(x, y, b);
			}
		}
};

class App: public SDLDevice {
	public:
		Context ctx;
		video_buffer_t data;
		int bpm = 120;
		
		App(int width, int height):
			SDLDevice(width, height, "Application"),
			data(rectangle_t(0, 0, width, height), COLOR_BG),
			ctx(dynamic_cast<SDLDevice&>(*this), data) {
				
		}
		void launch() {
			start();
			while(ctx.render()) {
				update(updatable);
				render(drawable);
				#ifdef SYSTEM_LINUX
				//usleep(60000/bpm/4 * 1000);
				#endif
			}
		}
		template <typename T>
		void add(T* object) {
			if (auto ptr = dynamic_cast<Drawable*>(object)) {drawable.push_back(ptr);}
			if (auto ptr = dynamic_cast<Updatable*>(object)) {updatable.push_back(ptr);}
			if (auto ptr = dynamic_cast<Clickable*>(object)) {clickable.push_back(ptr);}
		}
		~App() {
			stop();
		}
	private:
		int time = getTime();
		
		std::vector<Drawable*> drawable;
		std::vector<Updatable*> updatable;
		std::vector<Clickable*> clickable;
		
		int getTime() {	
			timeb tb;
			ftime(&tb);
			int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
			return nCount;
		}
		void update(std::vector<Updatable*>& updatable) {
			auto timeAfter = getTime();
			int delta = timeAfter - time;
			time = 2 * getTime() - timeAfter;
			/*if ((timeAfter - time) > 60000/bpm/4) {
				//printf("%d\n", timeAfter);
				time = 2 * getTime() - timeAfter;*/
				for (size_t i = 0; i < updatable.size(); i++) {
					updatable.at(i)->update(delta);
				}
			//}
		}
		void render(std::vector<Drawable*>& drawable) {
			ctx.clear(COLOR_BG);
			for (size_t i = 0; i < drawable.size(); i++) {
				ctx.draw(*(drawable.at(i)));
			}
		}
		/**
		 * Overloaded method for processing mouse buttons.
		 * @param button Index of button that triggered the event
		 * @param pressed true if button was pressed, false if released
		 * @return true, unles some fatal error has occured
		 */
		virtual bool do_mouse_button(const int button, const bool pressed, const int x, const int y)
		{
			if (pressed) {
				for (size_t i = 0; i < clickable.size(); i++) {
					Clickable* c = clickable.at(i);
					if (isIn(x, y, c->hitrectangle())) {
						c->click(x, y, button);
					}
				}
			}
			return true;
		}
};

int main() {
	App app(1024, 768);
	
	std::vector<std::string> captions;
	captions.push_back("DEVICE 1"); captions.push_back("DRUMS"); captions.push_back("KICK"); captions.push_back("SNARE"); captions.push_back("HAT"); captions.push_back("CLAP");
	ButtonGrid bg(25, 75, 16, captions);
	
	ControlPanel cp(0, 500, WIN_W, 100, &bg);
	
	app.add(&bg);
	app.add(&cp);
	
	app.launch();
}

