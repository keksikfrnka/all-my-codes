#include <cmath>
#include <stdio.h>
#include <iostream>

#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 10
#define L2G(X,Y) (l2g ((X), (Y), min_y, max_y))

int get_values(int n, int k, double* points, double* values);
double functions(int k, double x);
int newton_sdvig_coeffs(int n, int degree, double* points, double* values, double* coeffs);
double newton_sdvig_function(double x, int n, int degree, double* points, double* values, double* coeffs);
int piece_line_approx_min_square_coeffs(int n, double* points, double* values, double *coeffs);
double piece_line_approx_min_square(double x, double* points, double* values, double* coeffs);

int newton_sdvig_coeffs(int n, int degree, double* points, double* values, double* coeffs) {
    int i = 0, k = degree, j = k - 1;
    double **help = NULL;
    help = (double**)malloc((k - 1) * sizeof(double));
    for(i = 0; i < k - 1; i++) {
        help[i] = (double*)malloc(j * sizeof(double));
        j--;
    }
    for(j = k - 2; j >= 0; j--) {
        help[0][j] = (values[n + j - k + 1] - values[n + j - k]) / (points[n + j - k + 1] - points[n + j - k]);
    }
    for(i = 1; i < k - 1; i++) {
        for(j = k - 2 - i; j >= 0; j--) {
            help[i][j] = (help[i - 1][j + 1] - help[i - 1][j]) / (points[n - k + i + j + 1] - points[n - k + j]);
        }
    }
    for(i = 0; i < n - k; i ++) {
        coeffs[i * k] = (values[i + 1] - values[i]) / (points[i + 1] - points[i]);
    }
    for(j = 1; j < k; j++) {
        coeffs[(n - k - 1) * k + j] = (help[j - 1][0] - coeffs[(n - k - 1) * k + j - 1]) / (points[j + n - k] - points[n - k - 1]);
    }
    for(i = n - k - 2; i >= 0; i--) {
        for(j = 1; j < k; j++) {
            coeffs[i * k + j] = (coeffs[(i + 1) * k + j - 1] - coeffs[i * k + j - 1]) / (points[i + j + 1] - points[i]);
        }
    }
    for(i = 0; i < k - 1; i++) {
        free(help[i]);
    }
    free(help);
    return 0;
}

double newton_sdvig_function(double x, int n, int degree, double* points, double* values, double* coeffs) {
    int i = 0, j = 0, k = degree;
    double res = 0;
    while(x > points[i + 1] && i < n - k - 1) {
        i++;
    }
    for(j = k - 1; j >= 0; j--) {
        res = (x - points[i + j]) * (coeffs[i * k + j] + res);
    }
    return values[i] + res;
}

int piece_line_approx_min_square_coeffs(int n, double* points, double* values, double* coeffs) {
    int i;
    for(i = 0; i < n - 1; i++) {
        coeffs[i] = (values[i + 1] - values[i]) / (points[i + 1] - points[i]);
    }
    return 0;
}

double piece_line_approx_min_square(double x, double* points, double* values, double* coeffs) {
    int i = 0;
    double result = 0;
    while(x > points[i + 1]) {
        i++;
    }
    result = values[i] + (x - points[i]) * coeffs[i];
    return result;
}

double functions(int k, double x) {
    switch (k) {
        case 0 :
            return 1;
        case 1 :
            return x;
        case 2 :
            return x*x;
        case 3 :
            return x*x*x;
        case 4 :
            return x*x*x*x;
        case 5 :
            return exp(x);
        case 6 :
            return 1/(25*x*x+1);
        default :
            return -100000000;
    }
}

int get_values(int n, int k, double* points, double* values) {
    int i;
    for(i = 0; i < n; i++) {
        values[i] = functions(k, points[i]);
    }
    return 0;
}

static double f_0 (double x) {
    return x * 0 + 1;
}

static double f_1 (double x) {
    return x;
}

static double f_2 (double x) {
    return x * x;
}

static double f_3 (double x) {
    return x * x * x;
}

static double f_4 (double x) {
    return x * x * x * x;
}

static double f_5 (double x) {
    return exp(x);
}

static double f_6 (double x) {
    return 1 / (25 * x * x + 1);
}

Window::Window (QWidget *parent)
  : QWidget (parent)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  n = DEFAULT_N;

  func_id = 0;
  structure = 0;
  f_name = "f (x) = 1";
  f = f_0;
}

QSize Window::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize Window::sizeHint () const
{
  return QSize (1000, 1000);
}

int Window::parse_command_line (int argc, char *argv[])
{
  if (argc == 1)
    return 0;

  if (argc == 2)
    return -1;

  if (   sscanf (argv[1], "%lf", &a) != 1
      || sscanf (argv[2], "%lf", &b) != 1
      || b - a < 1.e-12
      || (argc > 3 && sscanf (argv[3], "%d", &n) != 1)
      || n <= 0)
    return -2;

  return 0;
}

// change current function for drawing
void Window::change_func() {
    func_id = (func_id + 1) % 7;
    switch (func_id) {
        case 0:
            f_name = "f (x) = 1";
            f = f_0;
            break;
        case 1:
            f_name = "f (x) = x";
            f = f_1;
            break;
        case 2:
            f_name = "f (x) = x^2";
            f = f_2;
            break;
        case 3:
            f_name = "f (x) = x^3";
            f = f_3;
            break;
        case 4:
            f_name = "f (x) = x^4";
            f = f_4;
            break;
        case 5:
            f_name = "f (x) = exp (x)";
            f = f_5;
            break;
        case 6:
            f_name = "f (x) = 1 / (25 * x^2 + 1)";
            f = f_6;
            break;
    }
  update ();
}

void Window::change_struct_func () {
    structure = (structure + 1) % 4;
    update ();
}

void Window::stretch_X() {
    double temp = (3 * a + b) / 4;
    b = (3 * b + a) / 4;
    a = temp;
    update();
}

void Window::compress_X() {
    double temp = (3 * a - b) / 2;
    b = (3 * b - a) / 2;
    a = temp;
    update();
}

void Window::increase_split() {
    n *= 2;
    update();
}

void Window::decrease_split() {
    n /= 2;
    if(n < 4) {
        n = 4;
    }
    update();
}

QPointF Window::l2g (double x_loc, double y_loc, double y_min, double y_max)
{
  double x_gl = (x_loc - a) / (b - a) * width ();
  double y_gl = (y_max - y_loc) / (y_max - y_min) * height ();
  return QPointF (x_gl, y_gl);
}

// render graph
void Window::paintEvent (QPaintEvent * /* event */) {
    QPainter painter (this);
    int degree = std::min(n / 2, 8), i;
    char outstr[40];
    double x1 = a, x2, x3 = a, x4, y1, y2, y3, y4, y5, y6;
    double max_y = 0, min_y = 0, loc_max = 0, loc_min = 0;
    double delta_x = (b - a) / (n - 1);
    double *points = NULL, *values = NULL, *coeffs = NULL, *coeffs2 = NULL;
    points = (double*)malloc(n * sizeof(double));
    values = (double*)malloc(n * sizeof(double));
    coeffs = (double*)malloc((n - degree) * degree * sizeof(double));
    coeffs2 = (double*)malloc((n - 1) * sizeof(double));
    if(points == NULL || values == NULL || coeffs == NULL) {
        free(coeffs2);
        free(coeffs);
        free(values);
        free(points);
        return;
    }

    QPen pen_black(Qt::black, 0, Qt::SolidLine);
    QPen pen_red(Qt::red, 0, Qt::SolidLine);
    QPen pen_magenta(Qt::magenta, 0, Qt::SolidLine);
    QPen pen_green(Qt::green, 0, Qt::SolidLine);

    painter.setPen (pen_black);

    for(i = 0; i < n; i++) {
        points[i] = a + i * delta_x;
    }
    get_values(n, func_id, points, values);

    if(structure < 3) {
        max_y = f(a);
        min_y = f(a);
        for (i = 0; i < n; i++) {
            x1 = points[i];
            y1 = f (x1);
            if (y1 < min_y) {
                min_y = y1;
            }
            if (y1 > max_y) {
                max_y = y1;
            }
        }
        loc_max = max_y;
        loc_min = min_y;
        if(max_y < min_y + 1e-12) {
            max_y += 0.5;
            min_y -= 0.5;
        }
        max_y += 0.01;
        min_y -= 0.01;
        x1 = a;
    }


    switch(structure) {
        case 0:
            newton_sdvig_coeffs(n, degree, points, values, coeffs);
            x1 = a;
            x3 = a;
            y1 = newton_sdvig_function(x1, n, degree, points, values, coeffs);
            y3 = f(x3);
            for(i = 1; i < n; i++) {
                x4 = points[i];
                y4 = f(x4);
                x2 = x4 - delta_x / 2;
                y2 = newton_sdvig_function(x2, n, degree, points, values, coeffs);
                painter.setPen (pen_black);
                painter.drawLine (L2G(x3, y3), L2G(x4, y4));
                painter.setPen(pen_magenta);
                painter.drawLine (L2G(x1, y1), L2G(x2, y2));
                x1 = x2;
                y1 = y2;
                x3 = x4;
                y3 = y4;
            }
            x2 = b;
            y2 = newton_sdvig_function(x2, n, degree, points, values, coeffs);
            painter.setPen(pen_magenta);
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));
            break;
        case 1:
            piece_line_approx_min_square_coeffs(n, points, values, coeffs2);
            x1 = a;
            x3 = a;
            y1 = piece_line_approx_min_square(x1, points, values, coeffs2);
            y3 = f(x3);
            for(i = 1; i < n; i++) {
                x4 = points[i];
                y4 = f(x4);
                x2 = x4 - delta_x / 2;
                y2 = piece_line_approx_min_square(x2, points, values, coeffs2);
                painter.setPen (pen_green);
                painter.drawLine (L2G(x1, y1), L2G(x2, y2));
                painter.setPen (pen_black);
                painter.drawLine (L2G(x3, y3), L2G(x4, y4));
                x1 = x2;
                y1 = y2;
                x3 = x4;
                y3 = y4;
            }
            x2 = b;
            y2 = piece_line_approx_min_square(x2, points, values, coeffs2);
            painter.setPen (pen_green);
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));
            break;
        case 2:
            newton_sdvig_coeffs(n, degree, points, values, coeffs);
            piece_line_approx_min_square_coeffs(n, points, values, coeffs2);
            x1 = a;
            x3 = a;
            y1 = newton_sdvig_function(x1, n, degree, points, values, coeffs);
            y5 = piece_line_approx_min_square(x1, points, values, coeffs2);
            y3 = f(x3);
            for(i = 1; i < n; i++) {
                x4 = points[i];
                y4 = f(x4);
                x2 = x4 - delta_x / 2;
                y2 = newton_sdvig_function(x2, n, degree, points, values, coeffs);
                y6 = piece_line_approx_min_square(x2, points, values, coeffs2);
                painter.setPen(pen_magenta);
                painter.drawLine (L2G(x1, y1), L2G(x2, y2));
                painter.setPen (pen_green);
                painter.drawLine (L2G(x1, y5), L2G(x2, y6));
                painter.setPen (pen_black);
                painter.drawLine (L2G(x3, y3), L2G(x4, y4));
                x1 = x2;
                y1 = y2;
                x3 = x4;
                y3 = y4;
                y5 = y6;
            }
            x2 = b;
            y2 = newton_sdvig_function(x2, n, degree, points, values, coeffs);
            painter.setPen(pen_magenta);
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));
            painter.setPen (pen_green);
            painter.drawLine (L2G(x1, y5), L2G(x2, y2));
            break;
        case 3:
            newton_sdvig_coeffs(n, degree, points, values, coeffs);
            piece_line_approx_min_square_coeffs(n, points, values, coeffs2);
            max_y = 0;
            min_y = 0;
            for(i = 0; i < n - 1; i++) {
                x1 = points[i] + delta_x / 2;
                y1 = abs(newton_sdvig_function(x1, n, degree, points, values, coeffs) - f(x1));
                y3 = abs(piece_line_approx_min_square(x1, points, values, coeffs2) - f(x1));
                if (y1 < min_y) {
                    min_y = y1;
                }
                if (y1 > max_y) {
                    max_y = y1;
                }
                if (y3 < min_y) {
                    min_y = y3;
                }
                if (y3 > max_y) {
                    max_y = y3;
                }
            }
            y1 = abs(newton_sdvig_function(b, n, degree, points, values, coeffs) - f(b));
            y3 = abs(piece_line_approx_min_square(b, points, values, coeffs2) - f(b));
            if (y1 < min_y) {
                min_y = y1;
            }
            if (y1 > max_y) {
                max_y = y1;
            }
            if (y3 < min_y) {
                min_y = y3;
            }
            if (y3 > max_y) {
                max_y = y3;
            }
            loc_max = max_y;
            loc_min = min_y;
            max_y = std::max(loc_max, 0.01);
            min_y -= 0.01;
            x1 = a;
            y1 = abs(newton_sdvig_function(x1, n, degree, points, values, coeffs) - f(x1));
            y3 = abs(piece_line_approx_min_square(x1, points, values, coeffs2) - f(x1));
            for(i = 0; i < n - 1; i++) {
                x2 = points[i] + delta_x / 2;
                y2 = abs(newton_sdvig_function(x2, n, degree, points, values, coeffs) - f(x2));
                y4 = abs(piece_line_approx_min_square(x2, points, values, coeffs2) - f(x2));
                painter.setPen(pen_magenta);
                painter.drawLine (L2G(x1, y1), L2G(x2, y2));
                painter.setPen (pen_green);
                painter.drawLine (L2G(x1, y3), L2G(x2, y4));
                x1 = x2;
                y1 = y2;
                y3 = y4;
            }
            x2 = b;
            y2 = abs(newton_sdvig_function(x2, n, degree, points, values, coeffs) - f(x2));
            painter.setPen(pen_magenta);
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));
            painter.setPen (pen_green);
            painter.drawLine (L2G(x1, y3), L2G(x2, y2));
            break;
    }


    // draw axis
    painter.setPen (pen_red);
    painter.drawLine (L2G(a, 0), L2G(b, 0));
    painter.drawLine (L2G(0, min_y), L2G(0, max_y));

    // render function name
    painter.setPen ("blue");
    sprintf(outstr, "k = %d     %s", func_id, f_name);
    painter.drawText (5, 20, outstr);
    sprintf(outstr, "structure = %d", structure);
    painter.drawText (5, 40, outstr);
    sprintf(outstr, "a = %lf", a);
    painter.drawText (5, 60, outstr);
    sprintf(outstr, "b = %lf", b);
    painter.drawText (5, 80, outstr);
    sprintf(outstr, "n = %d", n);
    painter.drawText (5, 100, outstr);
    sprintf(outstr, "y_max = %lf", loc_max);
    painter.drawText (5, 120, outstr);
    sprintf(outstr, "y_min = %lf", loc_min);
    painter.drawText (5, 140, outstr);

    free(coeffs2);
    free(coeffs);
    free(values);
    free(points);
}