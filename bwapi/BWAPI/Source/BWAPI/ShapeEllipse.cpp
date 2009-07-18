#include "ShapeEllipse.h"
#include "DLLMain.h"
#include "BWAPI/Game.h"
namespace BWAPI
{
	ShapeEllipse::ShapeEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, int color, bool isSolid)
			: ctype(ctype)
			, x(x)
			, y(y)
			, xrad(xrad)
			, yrad(yrad)
			, color(color)
			, isSolid(isSolid)
	{
	}

#define incx() x++, dxt += d2xt, t += dxt
#define incy() y--, dyt += d2yt, t += dyt

	void ShapeEllipse::draw()
	{
		if (this->isSolid)
		{
			int x = 0, y = this->yrad;
			unsigned int width = 1;
			long a2 = (long)this->xrad * this->xrad, b2 = (long)this->yrad * this->yrad;
			long crit1 = -(a2 / 4 + this->xrad % 2 + b2);
			long crit2 = -(b2 / 4 + this->yrad % 2 + a2);
			long crit3 = -(b2 / 4 + this->yrad % 2);
			long t = -a2 * y;
			long dxt = 2 * b2 * x, dyt = -2 * a2 * y;
			long d2xt = 2 * b2, d2yt = 2 * a2;

			while (y >= 0 && x <= this->xrad)
			{
				if (t + b2*x <= crit1 ||
				    t + a2*y <= crit3)
				{
					incx();
					width += 2;
				}
				else if (t - a2*y > crit2)
				{
					drawBox(this->x - x, this->y - y, width, 1, color, ctype);
					if (y != 0)
						drawBox(this->x - x, this->y + y, width, 1, color, ctype);
					incy();
				}
				else
				{
					drawBox(this->x - x, this->y - y, width, 1, color, ctype);
					if (y != 0)
						drawBox(this->x - x, this->y + y, width, 1, color, ctype);
					incx();
					incy();
					width += 2;
				}
			}
			if (this->yrad == 0)
				drawBox(this->x - this->xrad, this->y, 2*this->xrad + 1, 1, color, ctype);
		}
		else
		{
			int x = this->xrad;
			int y = 0;
			int twoAsquare = 2 * this->xrad * this->xrad;
			int twoBsquare = 2 * this->yrad * this->yrad;
			int xchange = this->yrad * this->yrad * (1 - 2 * this->xrad);
			int ychange = this->xrad * this->xrad;
			int ellipseerror = 0;
			int stoppingX = twoBsquare * this->xrad;
			int stoppingY = 0;

			while (stoppingX >= stoppingY)
			{
				drawDot(this->x + x, this->y + y, color, ctype);
				drawDot(this->x - x, this->y + y, color, ctype);
				drawDot(this->x - x, this->y - y, color, ctype);
				drawDot(this->x + x, this->y - y, color, ctype);

				y++;
				stoppingY += twoAsquare;
				ellipseerror += ychange;
				ychange += twoAsquare;
				if ((2*  ellipseerror + xchange) > 0)
				{
					x--;
					stoppingX -= twoBsquare;
					ellipseerror += xchange;
					xchange += twoBsquare;
				}
			}
			x = 0;
			y = this->yrad;
			xchange = this->yrad * this->yrad;
			ychange = this->xrad * this->xrad * (1 - 2 * this->yrad);
			ellipseerror = 0;
			stoppingX = 0;
			stoppingY = twoAsquare * this->yrad;
			while (stoppingX <= stoppingY)
			{

				drawDot(this->x + x, this->y + y, color, ctype);
				drawDot(this->x - x, this->y + y, color, ctype);
				drawDot(this->x - x, this->y - y, color, ctype);
				drawDot(this->x + x, this->y - y, color, ctype);

				x++;
				stoppingX += twoBsquare;
				ellipseerror += xchange;
				xchange += twoBsquare;
				if ((2*  ellipseerror + ychange) > 0)
				{
					y--;
					stoppingY -= twoAsquare;
					ellipseerror += ychange;
					ychange += twoAsquare;
				}
			}
		}
	}
}