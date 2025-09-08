#include <cmath>
#include <stdio.h>
#include <iostream>
#include <time.h>

#include "window.h"

#define DEFAULT_A_X -0.5
#define DEFAULT_A_Y -0.5
#define DEFAULT_B_X 0.5
#define DEFAULT_B_Y 0.5
#define DEFAULT_N_X 10
#define DEFAULT_N_Y 10
#define L2G(X,Y) (l2g ((X), (Y), a_y, b_y))


int newton_sdvig_2_var_coeffs(int n_x, int n_y, int degree_x, int degree_y, double* points_x, double* points_y, double** values, double**** coeffs) {
    int i = 0, i_x = 0, k_x = degree_x, j = 0, j_x = k_x, k_y = degree_y, i_y = 0;
    double ***help1 = NULL, ****help2 = NULL;
    help1 = (double***)malloc(n_y * sizeof(double));
    for(i = 0; i < n_y; i++) {
        help1[i] = (double**)malloc(k_x * sizeof(double));
        j_x = k_x;
        for(j = 0; j < k_x; j++) {
            help1[i][j] = (double*)malloc(j_x * sizeof(double));
            j_x--;
        }
    }
    j_x = k_y;
    help2 = (double****)malloc(k_y * sizeof(double));
    for(i = 0; i < k_y; i++) {
        help2[i] = (double***)malloc(j_x * sizeof(double));
        for(j = 0; j < j_x; j++) {
            help2[i][j] = (double**)malloc((n_x - k_x) * sizeof(double));
            for(i_x = 0; i_x < n_x - k_x; i_x++) {
                help2[i][j][i_x] = (double*)malloc((k_x + 1) * sizeof(double));
            }
        }
        j_x--;
    }
    for(i_y = 0; i_y < n_y - k_y; i_y++) {
        for(i = 0; i < k_x; i++) {
            help1[i_y][i][0] = values[n_x - k_x + i][i_y];
        }
        for(j = 1; j < k_x; j++) {
            for(i = k_x - 1 - j; i >= 0; i--) {
                help1[i_y][i][j] = (help1[i_y][i + 1][j - 1] - help1[i_y][i][j - 1]) / (points_x[n_x - k_x + i + j] - points_x[n_x - k_x + i]);
            }
        }
        for(i = 0; i < n_x - k_x; i++) {
            coeffs[i_y][0][i][0] = values[i][i_y];
        }
        for(j = 1; j < k_x + 1; j++) {
            coeffs[i_y][0][n_x - k_x - 1][j] = (help1[i_y][0][j - 1] - coeffs[i_y][0][n_x - k_x - 1][j - 1])/ (points_x[j + n_x - k_x - 1] - points_x[n_x - k_x - 1]);
        }
        for(i = n_x - k_x - 2; i >= 0; i--) {
            for(j = 1; j < k_x + 1; j++) {
                coeffs[i_y][0][i][j] = (coeffs[i_y][0][i + 1][j - 1] - coeffs[i_y][0][i][j - 1]) / (points_x[i + j] - points_x[i]);
            }
        }
    }
    for(i_y = n_y - k_y; i_y < n_y; i_y++) {
        for(i = 0; i < k_x; i++) {
            help1[i_y][i][0] = values[n_x - k_x + i][i_y];
        }
        for(j = 1; j < k_x; j++) {
            for(i = k_x - 1 - j; i >= 0; i--) {
                help1[i_y][i][j] = (help1[i_y][i + 1][j - 1] - help1[i_y][i][j - 1]) / (points_x[n_x - k_x + i + j] - points_x[n_x - k_x + i]);
            }
        }
        for(i = 0; i < n_x - k_x; i++) {
            help2[i_y - n_y + k_y][0][i][0] = values[i][i_y];
        }
        for(j = 1; j < k_x + 1; j++) {
            help2[i_y - n_y + k_y][0][n_x - k_x - 1][j] = (help1[i_y][0][j - 1] - help2[i_y - n_y + k_y][0][n_x - k_x - 1][j - 1])/ (points_x[j + n_x - k_x - 1] - points_x[n_x - k_x - 1]);
        }
        for(i = n_x - k_x - 2; i >= 0; i--) {
            for(j = 1; j < k_x + 1; j++) {
                help2[i_y - n_y + k_y][0][i][j] = (help2[i_y - n_y + k_y][0][i + 1][j - 1] - help2[i_y - n_y + k_y][0][i][j - 1]) / (points_x[i + j] - points_x[i]);
            }
        }
    }
    for(i_x = 0; i_x < n_x - k_x; i_x++) {
        for(j_x = 0; j_x < k_x + 1; j_x++) {
            for(j = 1; j < k_y; j++) {
                for(i = k_y - 1 - j; i >= 0; i--) {
                    help2[i][j][i_x][j_x] = (help2[i + 1][j - 1][i_x][j_x] - help2[i][j - 1][i_x][j_x]) / (points_y[n_y - k_y + i + j] - points_y[n_y - k_y + i]);
                }
            }
            for(j = 1; j < k_y + 1; j++) {
                coeffs[n_y - k_y - 1][j][i_x][j_x] = (help2[0][j - 1][i_x][j_x] - coeffs[n_y - k_y - 1][j - 1][i_x][j_x])/ (points_y[j + n_y - k_y - 1] - points_y[n_y - k_y - 1]);
            }
            for(i = n_y - k_y - 2; i >= 0; i--) {
                for(j = 1; j < k_y + 1; j++) {
                    coeffs[i][j][i_x][j_x] = (coeffs[i + 1][j - 1][i_x][j_x] - coeffs[i][j - 1][i_x][j_x]) / (points_y[i + j] - points_y[i]);
                }
            }
        }
    }
    j_x = k_y;
    for(i = 0; i < k_y; i++){
        for(j = 0; j < j_x; j++){
            for(i_x = 0; i_x < n_x - k_x; i_x++) {
                free(help2[i][j][i_x]);
            }
            free(help2[i][j]);
        }
        free(help2[i]);
        j_x--;
    }
    free(help2);
    for(i = 0; i < n_y; i++) {
        for(j = 0; j < k_x; j++) {
            free(help1[i][j]);
        }
        free(help1[i]);
    }
    free(help1);
    return 0;
}

double newton_sdvig_2_var_function(double x, double y, int n_x, int n_y, int degree_x, int degree_y, double* points_x, double* points_y, double**** coeffs) {
    int i = 0, j = 0, i_x = 0, i_y = 0, k_x = degree_x, k_y = degree_y;
    double res = 0, res1 = 0;
    while(x > points_x[i_x + 1] && i_x < n_x - k_x - 1) {
        i_x++;
    }
    while(y > points_y[i_y + 1] && i_y < n_y - k_y - 1) {
        i_y++;
    }
    for(i = k_y - 1; i >= 0; i--) {
        for(j = k_x - 1; j >= 0; j--) {
            res1 = (x - points_x[i_x + j]) * (coeffs[i_y][i + 1][i_x][j + 1] + res1);
        }
        res1 += coeffs[i_y][i + 1][i_x][0];
        res = (y - points_y[i_y + i]) * (res1 + res);
        res1 = 0;
    }
    for(j = k_x - 1; j >= 0; j--) {
            res1 = (x - points_x[i_x + j]) * (coeffs[i_y][0][i_x][j + 1] + res1);
    }
    res1 += coeffs[i_y][0][i_x][0];
    res += res1;
    return res;
}

static double f_0 (double x, double y) {
    return x * 0 + y * 0 + 1;
}

static double f_1 (double x, double y) {
    return x + y * 0;
}

static double f_2 (double x, double y) {
    return y + x * 0;
}

static double f_3 (double x, double y) {
    return x + y;
}

static double f_4 (double x, double y) {
    return sqrt(x * x + y * y);
}

static double f_5 (double x, double y) {
    return x * x + y * y;
}

static double f_6 (double x, double y) {
    return exp(x * x - y * y);
}

static double f_7 (double x, double y) {
    return 1 / (25 * (x * x + y * y) + 1);
}

Window::Window (QWidget *parent)
  : QWidget (parent)
{
    a_x = DEFAULT_A_X;
    a_y = DEFAULT_A_Y;
    b_x = DEFAULT_B_X;
    b_y = DEFAULT_B_Y;
    n_x = DEFAULT_N_X;
    n_y = DEFAULT_N_Y;
    func_id = 0;
    structure = 0;
    p = 0;
    f_name = "f (x,y) = 1";
    f = f_0;
}

int Window::parse_command_line (int argc, char *argv[]) {
    if (argc == 1) {
        return 0;
    }
    if (argc < 5) {
        return -1;
    }
    if (   sscanf (argv[1], "%lf", &a_x) != 1
        || sscanf (argv[2], "%lf", &a_y) != 1
        || sscanf (argv[3], "%lf", &b_x) != 1
        || sscanf (argv[4], "%lf", &b_y) != 1
        || b_x - a_x < 1.e-20
        || b_y - a_y < 1.e-20
        || (argc > 5 && sscanf (argv[5], "%d", &n_x) != 1)
        || (argc > 6 && sscanf (argv[6], "%d", &n_y) != 1)
        || n_x <= 0
        || n_y <= 0) {
        return -2;
    }
    return 0;
}

void Window::change_func_next() {
    func_id = (func_id + 1) % 8;
    switch (func_id) {
        case 0:
            f_name = "f (x,y) = 1";
            f = f_0;
            break;
        case 1:
            f_name = "f (x,y) = x";
            f = f_1;
            break;
        case 2:
            f_name = "f (x,y) = y";
            f = f_2;
            break;
        case 3:
            f_name = "f (x,y) = x + y";
            f = f_3;
            break;
        case 4:
            f_name = "f (x,y) = sqrt(x^2 + y^2)";
            f = f_4;
            break;
        case 5:
            f_name = "f (x,y) = x^2 + y^2";
            f = f_5;
            break;
        case 6:
            f_name = "f (x,y) = exp (x^2 - y^2)";
            f = f_6;
            break;
        case 7:
            f_name = "f (x,y) = 1 / (25 * (x^2 + y^2) + 1)";
            f = f_7;
            break;
    }
  update ();
}

void Window::change_func_back() {
    func_id = (func_id + 7) % 8;
    switch (func_id) {
        case 0:
            f_name = "f (x,y) = 1";
            f = f_0;
            break;
        case 1:
            f_name = "f (x,y) = x";
            f = f_1;
            break;
        case 2:
            f_name = "f (x,y) = y";
            f = f_2;
            break;
        case 3:
            f_name = "f (x,y) = x + y";
            f = f_3;
            break;
        case 4:
            f_name = "f (x,y) = sqrt(x^2 + y^2)";
            f = f_4;
            break;
        case 5:
            f_name = "f (x,y) = x^2 + y^2";
            f = f_5;
            break;
        case 6:
            f_name = "f (x,y) = exp (x^2 - y^2)";
            f = f_6;
            break;
        case 7:
            f_name = "f (x,y) = 1 / (25 * (x^2 + y^2) + 1)";
            f = f_7;
            break;
    }
  update ();
}

void Window::change_struct_func_next() {
    structure = (structure + 1) % 3;
    update ();
}

void Window::change_struct_func_back() {
    structure = (structure + 2) % 3;
    update ();
}

void Window::stretch_XY() {
    double temp = (3 * a_x + b_x) / 4;
    b_x = (3 * b_x + a_x) / 4;
    a_x = temp;
    temp = (3 * a_y + b_y) / 4;
    b_y = (3 * b_y + a_y) / 4;
    a_y = temp;
    update();
}

void Window::compress_XY() {
    double temp = (3 * a_x - b_x) / 2;
    b_x = (3 * b_x - a_x) / 2;
    a_x = temp;
    temp = (3 * a_y - b_y) / 2;
    b_y = (3 * b_y - a_y) / 2;
    a_y = temp;
    update();
}

void Window::increase_split() {
    n_x *= 2;
    n_y *= 2;
    update();
}

void Window::decrease_split() {
    n_x /= 2;
    if(n_x < 4) {
        n_x = 4;
    }
    n_y /= 2;
    if(n_y < 4) {
        n_y = 4;
    }
    update();
}

void Window::increase_perturbation() {
    p++;
    update();
}

void Window::decrease_perturbation() {
    p--;
    update();
}

QSize Window::minimumSizeHint() const {
    return QSize (100, 100);
}

QSize Window::sizeHint() const {
    return QSize (1000, 1000);
}

QPointF Window::l2g(double x_loc, double y_loc, double y_min, double y_max) {
    double x_gl = (x_loc - a_x) / (b_x - a_x) * width ();
    double y_gl = (y_max - y_loc) / (y_max - y_min) * height ();
    return QPointF (x_gl, y_gl);
}



void Window::paintEvent (QPaintEvent * /* event */) {
    QPainter painter (this);
    int degree_x = std::min(n_x / 2, 8), degree_y = std::min(n_y / 2, 8), i, j, i_x, i_y = 0;
    char outstr[50];
    double max_z, min_z, loc_max, loc_min, x, y, z, t, t2 = 0;
    double delta_x = (b_x - a_x) / (n_x - 1), delta_y = (b_y - a_y) / (n_y - 1);
    double *points_x = NULL, *points_y = NULL, **values = NULL, ****coeffs = NULL;
    t = clock();
    points_x = (double*)malloc(n_x * sizeof(double));
    points_y = (double*)malloc(n_y * sizeof(double));
    values = (double**)malloc(n_x * sizeof(double));
    for(i = 0; i < n_x; i++) {
        values[i] = (double*)malloc(n_y * sizeof(double));
    }
    coeffs = (double****)malloc((n_y - degree_y) * sizeof(double));
    for(i = 0; i < n_y - degree_y; i++) {
        coeffs[i] = (double***)malloc((degree_y + 1) * sizeof(double));
        for(j = 0; j < degree_y + 1; j++) {
            coeffs[i][j] = (double**)malloc((n_x - degree_x) * sizeof(double));
            for(i_x = 0; i_x < n_x - degree_x; i_x++) {
                coeffs[i][j][i_x] = (double*)malloc((degree_x + 1) * sizeof(double));
            }
        }
    }
    for(i = 0; i < n_x; i++) {
        if(values[i] == NULL) {
            i_y++;
        }
    }
    for(i = 0; i < n_y - degree_y; i++) {
        if(coeffs[i] == NULL) {
            i_y++;
        }
        for(j = 0; j < degree_y + 1; j++) {
            if(coeffs[i][j] == NULL) {
                i_y++;
            }
            for(i_x = 0; i_x < n_x - degree_x; i_x++) {
                if(coeffs[i][j][i_x] == NULL) {
                    i_y++;
                }
            }
        }
    }
    if(points_x == NULL || points_y == NULL || values == NULL || coeffs == NULL || i_y) {
        for(i = 0; i < n_y - degree_y; i++) {
            for(j = 0; j < degree_y + 1; j++) {
                for(i_x = 0; i_x < n_x - degree_x; i_x++) {
                    free(coeffs[i][j][i_x]);
                }
                free(coeffs[i][j]);
            }
            free(coeffs[i]);
        }
        free(coeffs);
        for(i = 0; i < n_x; i++) {
            free(values[i]);
        }
        free(values);
        free(points_y);
        free(points_x);
        return;
    }

    for(i = 0; i < n_x; i++) {
        points_x[i] = a_x + i * delta_x;
    }
    for(i = 0; i < n_y; i++) {
        points_y[i] = a_y + i * delta_y;
    }
    max_z = f(points_x[0], points_y[0]);
    min_z = max_z;
    for(i = 0; i < n_x; i++) {
        for(j = 0; j < n_y; j++) {
            z = f(points_x[i], points_y[j]);
            values[i][j] = z;
            if (z > max_z) {
                max_z = z;
            }
            if (z < min_z) {
                min_z = z;
            }
        }
    }
    values[(n_x - 1) / 2][(n_y - 1) / 2] += p * 0.1 * std::max(abs(max_z), abs(min_z));
    max_z = std::max(max_z, values[(n_x - 1) / 2][(n_y - 1) / 2]);
    min_z = std::min(min_z, values[(n_x - 1) / 2][(n_y - 1) / 2]);
    loc_max = max_z;
    loc_min = min_z;
    max_z += 0.01;

    switch(structure) {
        case 0:
            t2 = 0;
            for(i = 0; i < n_x - 1; i++) {
                for(j = 1; j < n_y; j++) {
                    painter.fillRect(QRect(L2G(points_x[i], points_y[j]).toPoint(), L2G(points_x[i + 1], points_y[j - 1]).toPoint()),
                        QColor::fromHsvF(std::min(std::max(f(points_x[i] + delta_x / 2, points_y[j - 1] + delta_y / 2) - min_z, 0.) / (max_z - min_z), 1.) * 5 / 6, 1, 1, 1));
                }
            }
            painter.fillRect(QRect(L2G(points_x[(n_x - 1) / 2 - 1], points_y[(n_y - 1) / 2 + 1]).toPoint(), L2G(points_x[(n_x - 1) / 2 + 1], points_y[(n_y - 1) / 2 - 1]).toPoint()),
                    QColor::fromHsvF(std::min(std::max(values[(n_x - 1) / 2][(n_y - 1) / 2] - min_z, 0.) / (max_z - min_z), 1.) * 5 / 6, 1, 1, 1));
            break;
        case 1:
            t2 = clock();
            newton_sdvig_2_var_coeffs(n_x, n_y, degree_x, degree_y, points_x, points_y, values, coeffs);
            t2 = clock() - t2;
            for(i = 0; i < n_x - 1; i++) {
                for(j = 1; j < n_y; j++) {
                    painter.fillRect(QRect(L2G(points_x[i], points_y[j]).toPoint(), L2G(points_x[i + 1], points_y[j - 1]).toPoint()),
                        QColor::fromHsvF(std::min(std::max(newton_sdvig_2_var_function(points_x[i] + delta_x / 2, points_y[j - 1] + delta_y / 2, n_x, n_y,
                            degree_x, degree_y, points_x, points_y, coeffs) - min_z, 0.) / (max_z - min_z), 1.) * 5 / 6, 1, 1, 1));
                }
            }
            break;
        case 2:
            t2 = clock();
            newton_sdvig_2_var_coeffs(n_x, n_y, degree_x, degree_y, points_x, points_y, values, coeffs);
            t2 = clock() - t2;
            max_z = 0;
            min_z = 0;
            for(i = 0; i < n_x - 1; i++) {
                for(j = 0; j < n_y - 1; j++) {
                    x = points_x[i] + delta_x / 2;
                    y = points_y[j] + delta_y / 2;
                    z = abs(newton_sdvig_2_var_function(x, y, n_x, n_y, degree_x, degree_y, points_x, points_y, coeffs) - f(x,y));
                    if (z > max_z) {
                        max_z = z;
                    }
                }
            }
            loc_max = max_z;
            loc_min = min_z;

            max_z = std::max(loc_max, 0.01);

            for(i = 0; i < n_x - 1; i++) {
                for(j = 1; j < n_y; j++) {
                    painter.fillRect(QRect(L2G(points_x[i], points_y[j]).toPoint(), L2G(points_x[i + 1], points_y[j - 1]).toPoint()),
                        QColor::fromHsvF(abs(newton_sdvig_2_var_function(points_x[i] + delta_x / 2, points_y[j - 1] + delta_y, n_x, n_y,
                            degree_x, degree_y, points_x, points_y, coeffs) - f(points_x[i] + delta_x / 2, points_y[j - 1] + delta_y)) / max_z * 5 / 6, 1, 1, 1));
                }
            }
            break;
    }


    // draw axis
    painter.setPen("black");
    painter.drawLine (L2G(a_x, 0), L2G(b_x, 0));
    painter.drawLine (L2G(0, a_y), L2G(0, b_y));

    // render function name
    sprintf(outstr, "k = %d     %s", func_id, f_name);
    painter.drawText (5, 20, outstr);
    sprintf(outstr, "structure = %d", structure);
    painter.drawText (5, 40, outstr);
    sprintf(outstr, "perturbation = %d", p);
    painter.drawText (5, 60, outstr);
    sprintf(outstr, "a_x = %lf", a_x);
    painter.drawText (5, 80, outstr);
    sprintf(outstr, "a_y = %lf", a_y);
    painter.drawText (5, 100, outstr);
    sprintf(outstr, "b_x = %lf", b_x);
    painter.drawText (5, 120, outstr);
    sprintf(outstr, "b_y = %lf", b_y);
    painter.drawText (5, 140, outstr);
    sprintf(outstr, "n_x = %d", n_x);
    painter.drawText (5, 160, outstr);
    sprintf(outstr, "n_y = %d", n_y);
    painter.drawText (5, 180, outstr);
    sprintf(outstr, "max = %lf", loc_max);
    painter.drawText (5, 200, outstr);
    sprintf(outstr, "min = %lf", loc_min);
    painter.drawText (5, 220, outstr);
    t = clock() - t;
    sprintf(outstr, "time for drawing = %lf", t / CLOCKS_PER_SEC);
    painter.drawText (5, 240, outstr);
    sprintf(outstr, "time for coeffs = %lf", t2 / CLOCKS_PER_SEC);
    painter.drawText (5, 260, outstr);
    for(i = 0; i < n_y - degree_y; i++) {
        for(j = 0; j < degree_y + 1; j++) {
            for(i_x = 0; i_x < n_x - degree_x; i_x++) {
                free(coeffs[i][j][i_x]);
            }
            free(coeffs[i][j]);
        }
        free(coeffs[i]);
    }
    free(coeffs);
    for(i = 0; i < n_x; i++) {
        free(values[i]);
    }
    free(values);
    free(points_y);
    free(points_x);
    return;
}   