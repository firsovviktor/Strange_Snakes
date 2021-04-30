#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#define PI 3.14159265
int AspectRatio = 4;
float Snake_speed = 0.01;
bool invisible = true;
using namespace std;
using namespace sf;

struct Tile {
    Vector2f center;
    Color countour;
    Color inside;
    int thick;
    vector<Vector2f> vert;
    vector<Tile*> close; // w, s, a, d

    Tile(Vector2f center, Color countour, Color inside,
         int thick, vector<Vector2f> vert, vector<Tile*> close) : center(center),
                                                                  countour(countour),
                                                                  inside(inside),
                                                                  thick(thick),
                                                                  vert(vert),
                                                                  close(close) {}
};
struct Snake{
    Color countour;
    Color countour_head;
    Color inside;
    Color inside_head;
    int thick;
    int thick_head;
    vector<Vector2f> vert;
    float time;
    Snake() = default;
};
struct Apple {
    Tile tile;
    float radius;
    Color fill;
    Color countour;
    int thick;
    bool is_eaten;
    Apple(Tile tile, float radius, Color fill, Color countour,
          int thick, bool is_eaten): tile(tile),
                                     radius(radius),
                                     fill(fill),
                                     countour(countour),
                                     thick(thick),
                                     is_eaten(is_eaten){}
};

ConvexShape draw_tile(const Tile& figure){
    ConvexShape convex;

    convex.setPointCount(figure.vert.size());

    for (int i = 0; i < figure.vert.size(); i++) {
        convex.setPoint(i, figure.vert[i] + figure.center);
    }

    convex.setFillColor(figure.inside);
    convex.setOutlineThickness(figure.thick);
    convex.setOutlineColor(figure.countour);
    return convex;
}

RectangleShape draw_snake(const Snake& s, int i, int step){
    RectangleShape r;
    float dist2 = (s.vert[i+1].x-s.vert[i].x)*(s.vert[i+1].x-s.vert[i].x) + (s.vert[i+1].y-s.vert[i].y)*(s.vert[i+1].y-s.vert[i].y);
    Vector2f vec, vec2;
    vec.x = float(step/2/AspectRatio);
    vec.y = float(step/2/AspectRatio);
    r.setSize({sqrt(dist2), step/AspectRatio});
    float angle;
    angle = atan2(s.vert[i+1].y-s.vert[i].y,s.vert[i+1].x-s.vert[i].x);
    vec2.x = vec.x*cos(angle)+vec.y*sin(angle);
    vec2.y = vec.y*sin(angle)-vec.y*cos(angle);
    r.setRotation(angle*180/PI);
    r.setPosition(s.vert[i] + vec2);
    r.setFillColor(s.inside);
    r.setOutlineThickness(s.thick);
    r.setOutlineColor(s.countour);
    if ((sqrt((s.vert[i+1].x-s.vert[i].x)*(s.vert[i+1].x-s.vert[i].x) + (s.vert[i+1].y-s.vert[i].y)*(s.vert[i+1].y-s.vert[i].y)) > step * 6)&&(invisible == true)){
        r.setFillColor({0, 0, 0, 0});
        r.setOutlineColor({0, 0, 0, 0});
    }
    return r;
}
RectangleShape draw_snake_head(const Snake& s, int step, float time){
    RectangleShape r;
    int i = s.vert.size()-2;
    float dist2 = (s.vert[i+1].x-s.vert[i].x)*(s.vert[i+1].x-s.vert[i].x) + (s.vert[i+1].y-s.vert[i].y)*(s.vert[i+1].y-s.vert[i].y);
    dist2 = dist2 * time * time;
    Vector2f vec, vec2;
    vec.x = float(step/2/AspectRatio);
    vec.y = float(step/2/AspectRatio);
    r.setSize({sqrt(dist2), step/AspectRatio});
    float angle;
    angle = atan2(s.vert[i+1].y-s.vert[i].y,s.vert[i+1].x-s.vert[i].x);
    vec2.x = vec.x*cos(angle)+vec.y*sin(angle);
    vec2.y = vec.y*sin(angle)-vec.y*cos(angle);
    r.setRotation(angle*180/PI);
    r.setPosition(s.vert[i] + vec2);
    r.setFillColor(s.inside_head);
    r.setOutlineThickness(s.thick_head);
    r.setOutlineColor(s.countour_head);
    if ((sqrt((s.vert[i+1].x-s.vert[i].x)*(s.vert[i+1].x-s.vert[i].x) + (s.vert[i+1].y-s.vert[i].y)*(s.vert[i+1].y-s.vert[i].y)) > step * 6)&&(invisible==true)){
        r.setFillColor({0, 0, 0, 0});
        r.setOutlineColor({0, 0, 0, 0});
    }
    return r;
}
RectangleShape draw_snake_tail(const Snake& s, int step, float time){
    RectangleShape r;
    int i = 0;
    float dist2 = (s.vert[i+1].x-s.vert[i].x)*(s.vert[i+1].x-s.vert[i].x) + (s.vert[i+1].y-s.vert[i].y)*(s.vert[i+1].y-s.vert[i].y);
    dist2 = dist2 * (1-time) * (1-time);
    Vector2f vec, vec2, off;
    vec.x = float(step/2/AspectRatio);
    vec.y = float(step/2/AspectRatio);
    r.setSize({sqrt(dist2), step/AspectRatio});
    float angle;
    angle = atan2(s.vert[i+1].y-s.vert[i].y,s.vert[i+1].x-s.vert[i].x);
    vec2.x = vec.x*cos(angle)+vec.y*sin(angle);
    vec2.y = vec.y*sin(angle)-vec.y*cos(angle);
    r.setRotation(angle*180/PI);
    r.setPosition(s.vert[i] + vec2);
    off = s.vert[i+1]-s.vert[i];
    off = off * time;
    r.move(off);
    r.setFillColor(s.inside);
    r.setOutlineThickness(s.thick);
    r.setOutlineColor(s.countour);
    if ((sqrt((s.vert[i+1].x-s.vert[i].x)*(s.vert[i+1].x-s.vert[i].x) + (s.vert[i+1].y-s.vert[i].y)*(s.vert[i+1].y-s.vert[i].y)) > step * 6)&&(invisible == true)){
        r.setFillColor({0, 0, 0, 0});
        r.setOutlineColor({0, 0, 0, 0});
    }
    return r;
}

vector<Tile> gen_square_field(int Width, int Height, int step, Tile t){
    vector<Tile> field;
    for (int i=0; i<Width/step; i++){
        for (int j=0; j<Height/step; j++){
            t.center = {step*i+step/2, step*j+step/2};
            t.vert.clear();
            t.vert.push_back({-step/2, -step/2});
            t.vert.push_back({-step/2, step/2});
            t.vert.push_back({step/2, step/2});
            t.vert.push_back({step/2, -step/2});
            t.close = {};
            field.push_back(t);
        }
    }
    return field;
}
vector<Tile> near_sq_field(vector<Tile> field, int Height, int Width, int step){
    for (int i=0; i<Width/step; i++){
        for (int j=0; j<Height/step; j++){
            Tile t = field[i*Height/step + j];
            if (j==0){
                t.close.push_back(&field[Height/step * (i+1) - 1]);
            }else{
                t.close.push_back(&field[Height/step * i + j - 1]);
            }
            //cout << i*Height/step + j << ' ' << t.close[0]->center.x << ' ' << t.close[0]->center.y << endl;
            field[i*Height/step + j] = t;
        }
    }
    for (int i=0; i<Width/step; i++){
        for (int j=0; j<Height/step; j++){
            Tile t = field[i*Height/step + j];
            if (j==Height/step-1){
                t.close.push_back(&field[Height/step * i]);
            }else{
                t.close.push_back(&field[Height/step * i + j + 1]);
            }
            //cout << i*Height/step + j << ' ' << t.close[1]->center.x << ' ' << t.close[1]->center.y << endl;
            field[i*Height/step + j] = t;
        }
    }
    for (int i=0; i<Width/step; i++){
        for (int j=0; j<Height/step; j++){
            Tile t = field[i*Height/step + j];
            if (i==0){
                t.close.push_back(&field[Height/step * (Width/step - 1) + j]);
            }else{
                t.close.push_back(&field[Height/step * i + j - Height/step]);
            }
            //cout << i*Height/step + j << ' ' << t.close[2]->center.x << ' ' << t.close[2]->center.y << endl;
            field[i*Height/step + j] = t;
        }
    }
    for (int i=0; i<Width/step; i++){
        for (int j=0; j<Height/step; j++){
            Tile t = field[i*Height/step + j];
            if (i==Width/step-1){
                t.close.push_back(&field[j]);
            }else{
                t.close.push_back(&field[Height/step * i + j + Height/step]);
            }
            //cout << i*Height/step + j << ' ' << t.close[0]->center.x << ' ' << t.close[0]->center.y << endl;
            field[i*Height/step + j] = t;
        }
    }
    return field;
}

vector<Tile> gen_triangle_field (int Width, int Height, int step, Tile tile){
    int a = Width/step - 1;
    int r = Height/(sqrt(3)*step);
    Tile t = tile;
    vector<Tile> field;
    field.clear();
    float f = sqrt(3)/6;
    int j;
    //cout << "beginning of gen" << endl;
    for (int i=0; i<r; i++){
        for (int jj=0; jj<4*a; jj++){
            //cout << "b" << endl;
            //cout << "generating " << i << " of " << r << "; " << jj << " of " << 4*a << ";" << endl;
            j = jj/4;
            if (jj%4 == 0){
                t.center = {j * step + step / 2, 6 * f * i * step + f * step};
                t.vert.clear();
                t.vert.push_back({-step / 2, -step * f});
                t.vert.push_back({step / 2, -step * f});
                t.vert.push_back({0, step * 2 * f});
                //t.inside = {255, 0, 0, 0};
                t.close = {};
                field.push_back(t);
            }
            if (jj%4 == 1){
                t.center = {j*step + step, 6 * f * i * step + 2 * f * step};
                t.vert.clear();
                t.vert.push_back({0, -2*f*step});
                t.vert.push_back({step/2, f*step});
                t.vert.push_back({-step/2, f*step});
                //t.inside = {0, 255, 0, 0};
                t.close = {};
                //cout << t.vert[0].x << " " << t.vert[0].y << "/";
                //cout << t.vert[1].x << " " << t.vert[1].y << "/";
                //cout << t.vert[2].x << " " << t.vert[2].y << "/" << endl;
                field.push_back(t);
            }
            if (jj%4 == 2){
                t.center = {j*step + step, 6 * f * i * step + 4 * f * step};
                t.vert.clear();
                t.vert.push_back({-step / 2, -step * f});
                t.vert.push_back({step / 2, -step * f});
                t.vert.push_back({0, step * 2 * f});
                //t.inside = {0, 0, 255, 0};
                t.close = {};
                field.push_back(t);
            }
            if (jj%4 == 3){
                t.center = {j*step + step/2, 6 * f * i * step + 5 * f * step};
                t.vert.clear();
                t.vert.push_back({0, -2*f*step});
                t.vert.push_back({step/2, f*step});
                t.vert.push_back({-step/2, f*step});
                //t.inside = {0, 0, 0, 0};
                t.close = {};
                field.push_back(t);
            }
        }
    }
    //cout << "end of gen" << endl;
    return field;
}
vector<Tile> near_tr_field(vector<Tile> field, int Height, int Width, int step){
    int a = Width/step - 1;
    int r = Height/(sqrt(3)*step);
    int j, cur;
    for (int i=0; i<r; i++) {
        for (int jj = 0; jj < 4 * a; jj++) {
            j = jj / 4;
            cur = 4*a*i+jj;
            if (jj%4 == 0){
                if (i>0){
                    field[cur].close.push_back(&(field[cur-4*a+3]));
                }else{
                    field[cur].close.push_back(&(field[(r-1)*4*a+3]));
                }
                if (j>0){
                    field[cur].close.push_back(&(field[cur-3]));
                }else{
                    field[cur].close.push_back(&field[cur+4*a-3]);
                }
                field[cur].close.push_back(&(field[cur+1]));
                //cout << "/" << cur << "/" << field[cur].close.size() << "/" << field[cur].close[0]->center.x << " " << field[cur].close[0]->center.y;
                //cout << "/" << field[cur].close[1]->center.x << " " << field[cur].close[1]->center.y;
                //cout << "/" << field[cur].close[2]->center.x << " " << field[cur].close[2]->center.y << "/" << endl;
            }
            if (jj%4 == 1){
                if ((cur+3)%(4*a)==0){
                    field[cur].close.push_back(&(field[cur+3-4*a]));
                }else{
                    field[cur].close.push_back(&(field[cur+3]));
                }
                field[cur].close.push_back(&(field[cur-1]));
                field[cur].close.push_back(&(field[cur+1]));
                //cout << "/" << cur << "/" << field[cur].close.size() << "/" << field[cur].close[0]->center.x << " " << field[cur].close[0]->center.y;
                //cout << "/" << field[cur].close[1]->center.x << " " << field[cur].close[1]->center.y;
                //cout << "/" << field[cur].close[2]->center.x << " " << field[cur].close[2]->center.y << "/" << endl;
            }
            if (jj%4 == 2){
                field[cur].close.push_back(&(field[cur-1]));
                field[cur].close.push_back(&(field[cur+1]));
                if ((cur+2)%(4*a)==0){
                    field[cur].close.push_back(&(field[cur+5-4*a]));
                }else{
                    field[cur].close.push_back(&(field[cur+5]));
                }
                //cout << "/" << cur << "/" << field[cur].close.size() << "/" << field[cur].close[0]->center.x << " " << field[cur].close[0]->center.y;
                //cout << "/" << field[cur].close[1]->center.x << " " << field[cur].close[1]->center.y;
                //cout << "/" << field[cur].close[2]->center.x << " " << field[cur].close[2]->center.y << "/" << endl;
            }
            if (jj%4 == 3){
                field[cur].close.push_back(&(field[cur-1]));
                if ((cur-3)%(4*a)==0){
                    field[cur].close.push_back(&(field[cur+4*a-5]));
                }else{
                    field[cur].close.push_back(&(field[cur-5]));
                }
                if (i==r-1){
                    field[cur].close.push_back(&(field[(cur+5)%(4*a)]));
                }else{
                    field[cur].close.push_back(&(field[cur+4*a-3]));
                }
                //cout << "/" << cur << "/" << field[cur].close.size() << "/" << field[cur].close[0]->center.x << " " << field[cur].close[0]->center.y;
                //cout << "/" << field[cur].close[1]->center.x << " " << field[cur].close[1]->center.y;
                //cout << "/" << field[cur].close[2]->center.x << " " << field[cur].close[2]->center.y << "/" << endl;
            }
        }
    }
    return field;
}

vector<Tile> gen_hex_field(int Width, int Height, int step, Tile t, int r, int a){
    float f = sqrt(3)/6;
    vector<Tile> field;
    field.clear();

    for (int i=0; i<r; i++){
        for (int j=0; j<a; j++){
            t.center = {step*f*3 + step*j*6*f, step + 3*step*i};
            t.vert.clear();
            t.vert.push_back({0, -step});
            t.vert.push_back({step*3*f, -step/2});
            t.vert.push_back({step*3*f, step/2});
            t.vert.push_back({0, step});
            t.vert.push_back({-step*3*f, step/2});
            t.vert.push_back({-step*3*f, -step/2});
            t.close = {};
            field.push_back(t);
            //cout << t.center.x << " " << t.center.y << endl;
        }
        for (int j=0; j<a; j++){
            t.center = {step*f*6 + step*j*6*f, 5*step/2 + 3*step*i};
            t.vert.clear();
            t.vert.push_back({0, -step});
            t.vert.push_back({step*3*f, -step/2});
            t.vert.push_back({step*3*f, step/2});
            t.vert.push_back({0, step});
            t.vert.push_back({-step*3*f, step/2});
            t.vert.push_back({-step*3*f, -step/2});
            t.close = {};
            field.push_back(t);
            //cout << t.center.x << " " << t.center.y << endl;
        }
    }
    return field;
}
vector<Tile> near_hex_field(vector<Tile> field, int Height, int Width, int step, int r, int a){
    //cout << "The very beginning of near" << endl;

    float f = sqrt(3)/6;
    int cur;

    //cout << "before near cycle" << endl;
    //cout << a << " " << r << endl;

    for (int i=0; i<r; i++){
        for (int j=0; j<a; j++){
            cur = i*2*a + j;

            //cout << "In the first cycle, now at: " << i << "/" << r << " " << j << "/" << a << endl;

            // 0 - heading
            if ((i==0)&&(j==0)){
                field[cur].close.push_back(&(field[r*2*a-1]));
            }else{
                if (i==0){
                    field[cur].close.push_back(&(field[r*2*a-a + j - 1]));
                }
                if (j==0){
                    field[cur].close.push_back(&(field[cur-1]));
                }
                if ((i!=0)&&(j!=0)){
                    field[cur].close.push_back(&(field[cur - a - 1]));
                }
            }

            //1 - heading
            if (i==0){
                field[cur].close.push_back(&(field[2*r*a - a + j]));
            }else{
                field[cur].close.push_back(&(field[cur - a]));
            }

            //2 - heading, universal
            if (j==0){
                field[cur].close.push_back(&(field[cur + a - 1]));
            }else{
                field[cur].close.push_back(&(field[cur - 1]));
            }

            //3 - heading, universal
            if (j==a-1){
                field[cur].close.push_back(&(field[cur - a + 1]));
            }else{
                field[cur].close.push_back(&(field[cur + 1]));
            }

            //4 - heading
            if (j==0){
                field[cur].close.push_back(&(field[cur + 2*a - 1]));
            }else{
                field[cur].close.push_back(&(field[cur + a - 1]));
            }

            //5 - heading
            field[cur].close.push_back(&(field[cur + a]));

            //cout << "0: " << field[cur].close[0]->center.x << " " << field[cur].close[0]->center.y << endl;
            //cout << "1: " << field[cur].close[1]->center.x << " " << field[cur].close[1]->center.y << endl;
            //cout << "2: " << field[cur].close[2]->center.x << " " << field[cur].close[2]->center.y << endl;
            //cout << "3: " << field[cur].close[3]->center.x << " " << field[cur].close[3]->center.y << endl;
            //cout << "4: " << field[cur].close[4]->center.x << " " << field[cur].close[4]->center.y << endl;
            //cout << "5: " << field[cur].close[5]->center.x << " " << field[cur].close[5]->center.y << endl;
        }

        for (int j=0; j<a; j++){
            cur = i*2*a + a + j;

            //cout << "In the second cycle, now at: " << i << "/" << r << " " << j << "/" << a << endl;

            // 0 - heading
            field[cur].close.push_back(&(field[cur - a]));

            // 1 - heading
            if (j == a-1){
                field[cur].close.push_back(&(field[cur - 2*a + 1]));
            }else{
                field[cur].close.push_back(&(field[cur - a + 1]));
            }

            //2 - heading, universal
            if (j==0){
                field[cur].close.push_back(&(field[cur + a - 1]));
            }else{
                field[cur].close.push_back(&(field[cur - 1]));
            }

            //3 - heading, universal
            if (j == a-1){
                field[cur].close.push_back(&(field[cur - a + 1]));
            }else{
                field[cur].close.push_back(&(field[cur + 1]));
            }

            // 4 - heading
            if (i == r-1){
                field[cur].close.push_back(&(field[j]));
            }else{
                field[cur].close.push_back(&(field[cur + a]));
            }

            // 5 - heading
            if ((i == r-1)&&(j == a-1)){
                field[cur].close.push_back(&(field[0]));
            }else{
                if (i==r-1){
                    field[cur].close.push_back(&(field[j+1]));
                }
                if (j==a-1){
                    field[cur].close.push_back(&(field[cur+1]));
                }
                if ((i!=r-1)&&(j!=a-1)){
                    //cout << "success" << endl;
                    field[cur].close.push_back(&(field[cur+a+1]));
                }
            }
            //cout << "0: " << field[cur].close[0]->center.x << " " << field[cur].close[0]->center.y << endl;
            //cout << "1: " << field[cur].close[1]->center.x << " " << field[cur].close[1]->center.y << endl;
            //cout << "2: " << field[cur].close[2]->center.x << " " << field[cur].close[2]->center.y << endl;
            //cout << "3: " << field[cur].close[3]->center.x << " " << field[cur].close[3]->center.y << endl;
            //cout << "4: " << field[cur].close[4]->center.x << " " << field[cur].close[4]->center.y << endl;
            //cout << "5: " << field[cur].close[5]->center.x << " " << field[cur].close[5]->center.y << endl;
        }
    }
    return field;
}

bool check_if_dead (Snake s){
    for (int i=0; i<s.vert.size(); i++){
        for (int j=i; j<s.vert.size(); j++){
            if (i==j){
            }else{
                if (s.vert[i] == s.vert[j]){
                    //cout << i << " /" << s.vert[i].x << " /" << s.vert[i].y << " /" << endl;
                    //cout << j << " /" << s.vert[i].x << " /" << s.vert[i].y << " /" << endl;
                    return true;
                }
            }
        }
    }
    return false;
}
void win_screen(int score){
    cout << "You have won! Your score is: " << score << "!" << endl;
}

int main() {
    srand(time(NULL));
    // Устанавливаем 8-й уровень сглаживания
    bool interface = true;
    string type_of_field = "Square";
    int Width = 1000, Height = 800, step = 40;
    Tile Prototype({0, 0}, {255, 1, 1, 255}, {1, 255, 1, 50}, 1, {{10, 30}, {10, 100}, {50, 70}}, {});
    Snake s;

    s.countour = {128, 1, 128, 255};
    s.countour_head = {1, 1, 255, 255};
    s.inside = {1, 255, 1, 255};
    s.inside_head = {1, 255, 1, 255};
    s.thick = 2;
    s.thick_head = 2;
    s.vert.clear();
    s.time = 0;

    ContextSettings settings;
    settings.antialiasingLevel = 8;

    string Difficulty = "None";

    if (interface){

        Font font;

        if (!font.loadFromFile("arial.ttf"))
        {
            // error...
        }

        RectangleShape sandbox, normal, hard, set, start;
        sandbox.setOutlineColor({0, 0, 0, 255});
        normal.setOutlineColor({0, 0, 0, 255});
        hard.setOutlineColor({0, 0, 0, 255});
        set.setOutlineColor({0, 0, 0, 255});
        start.setOutlineColor({0, 0, 0, 255});

        sandbox.setOutlineThickness(5);
        normal.setOutlineThickness(5);
        hard.setOutlineThickness(5);
        set.setOutlineThickness(5);
        start.setOutlineThickness(5);

        sandbox.setFillColor({0, 188, 0, 100});
        normal.setFillColor({0, 0, 188, 100});
        hard.setFillColor({188, 0, 0, 100});
        set.setFillColor({0, 188, 0, 100});
        start.setFillColor({0, 188, 0, 100});

        sandbox.setSize({180, 60});
        sandbox.setPosition(235, 160);

        normal.setSize({160, 60});
        normal.setPosition(430, 160);

        hard.setSize({160, 60});
        hard.setPosition(605, 160);

        set.setSize({680, 60});
        set.setPosition(60, 310);

        start.setSize({280, 100});
        start.setPosition(250, 440);

        Text text;
        text.setFont(font);
        text.setString("Hello world");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setStyle(sf::Text::Bold);

        RenderWindow interface(VideoMode(800, 600), "Settings screen!", Style::Default, settings);
        interface.clear(Color::White);

        string Button_pressed;

        int x, y;

        while(interface.isOpen()){
            Event event;
            while (interface.pollEvent(event)) {
                if (event.type == Event::Closed){
                    interface.close();
                }

                if (event.type == Event::MouseButtonPressed){
                    //cout << event.mouseButton.x << " " << event.mouseButton.y << endl;
                    x = event.mouseButton.x;
                    y = event.mouseButton.y;
                }
            }

            interface.clear(Color::White);

            Button_pressed = "None";
            if ((x>=235)&&(x<=235+180)){
                if ((y>=160)&&(y<=160+60)){
                    Button_pressed = "Sandbox";
                    Difficulty = "Sandbox";
                }
            }
            if ((x>=430)&&(x<=430+160)){
                if ((y>=160)&&(y<=160+60)){
                    Button_pressed = "Normal";
                    Difficulty = "Normal";
                }
            }
            if ((x>=605)&&(x<=605+160)){
                if ((y>=160)&&(y<=160+60)){
                    Button_pressed = "Hard";
                    Difficulty = "Hard";
                }
            }
            if ((x>=60)&&(x<=60+680)){
                if ((y>=310)&&(y<=310+60)){
                    Button_pressed = "set";
                }
            }
            if ((x>=250)&&(x<=250+280)){
                if ((y>=440)&&(y<=440+100)){
                    Button_pressed = "start";
                }
            }
            if (Difficulty!="None"){
                sandbox.setFillColor({0, 188, 0, 100});
                normal.setFillColor({0, 0, 188, 100});
                hard.setFillColor({188, 0, 0, 100});

                if (Difficulty == "Sandbox"){
                    sandbox.setFillColor({0, 255, 0, 255});
                }

                if (Difficulty == "Normal"){
                    normal.setFillColor({0, 0, 255, 255});
                }

                if (Difficulty == "Hard"){
                    hard.setFillColor({255, 0, 0, 255});
                }
            }

            if (Button_pressed == "set"){
                Difficulty = "set";
                interface.close();
            }

            if (Button_pressed == "start"){
                interface.close();
            }

            interface.draw(sandbox);
            interface.draw(normal);
            interface.draw(hard);
            interface.draw(set);

            interface.draw(start);
            text.setString("SETTINGS");
            text.setCharacterSize(60);
            text.setFillColor({0, 0, 0, 255});
            text.setPosition(230, 30);
            interface.draw(text);

            text.setString("Difficulty:");
            text.setCharacterSize(40);
            text.setFillColor({0, 0, 0, 255});
            text.setPosition(20, 160);
            interface.draw(text);

            text.setString("Sandbox");
            text.setCharacterSize(40);
            text.setFillColor({0, 0, 0, 255});
            text.setPosition(240, 160);
            interface.draw(text);

            text.setString("Normal");
            text.setCharacterSize(40);
            text.setFillColor({0, 0, 0, 255});
            text.setPosition(440, 160);
            interface.draw(text);

            text.setString("Hard");
            text.setCharacterSize(40);
            text.setFillColor({0, 0, 0, 255});
            text.setPosition(640, 160);
            interface.draw(text);

            text.setString("Advanced settings through console");
            text.setCharacterSize(40);
            text.setFillColor({0, 0, 0, 255});
            text.setPosition(70, 310);
            interface.draw(text);

            text.setString("START");
            text.setCharacterSize(80);
            text.setFillColor({0, 0, 0, 255});
            text.setPosition(260, 440);
            interface.draw(text);

            interface.display();
        }
    }

    if (Difficulty == "None"){
        Difficulty = "Sandbox";
    }

    if (Difficulty == "set"){

        cout << "You have chosen to open an advanced settings." << endl;
        cout << "Enter resolution with a space between. Width first, then height." << endl;
        cout << "800 800 or more is recommended." << endl;
        cin >> Width >> Height;

        cout << "Chose size of a tile. 40 is recommednded." << endl;
        cin >> step;

        cout << "Chose type of filed. Square/Triangle/Hex." << endl;
        cin >> type_of_field;

        cout << "Chose your difficulty. Sandbox/Normal/Hard" << endl;
        cin >> Difficulty;

        cout << "Chose initial speed of the snake. 0.002 - 0.05 is recommended." << endl;
        cin >> Snake_speed;

        cout << "Chose ratio of tile size to the snake size (an integer). 4 is recommended" << endl;
        cin >> AspectRatio;

        cout << "Chose if you want the snake to be invisible while going over the edge of the board (0/1). 1 is recommended" << endl;
        cin >> invisible;
    }

    int r = (Height/step - 1)/3;
    float fl = Width/sqrt(3);
    int a = floor(fl)/step - 1;

    cout << "Starting a game" << endl;
    // Объект, который, собственно, является главным окном приложения

    RenderWindow window(VideoMode(Width, Height), "SFML Works!", Style::Default, settings);
    window.clear(Color::White);

    vector<Tile> field;

    if (type_of_field == "Square"){
        field = gen_square_field(Width, Height, step, Prototype);
        field = near_sq_field(field, Height, Width, step);
    }
    if (type_of_field == "Triangle"){
        field = gen_triangle_field(Width, Height, step, Prototype);
        field = near_tr_field(field, Height, Width, step);
    }
    //cout << "before field" << endl;
    if (type_of_field == "Hex"){
        field = gen_hex_field(Width, Height, step, Prototype, r, a);
        //cout << "after gen, before near" << endl;
        field = near_hex_field(field, Height, Width, step, r, a);
    }
    //cout << "after field" << endl;
    s.vert.push_back(field[0].center);
    s.vert.push_back(field[1].center);
    s.vert.push_back(field[2].center);
    //s.vert.push_back(field[3].center);

    Tile cur = field[2];

    //cout << "snake initialized" << endl;

    int key;
    if (type_of_field == "Square"){
        key = 1;
    }
    if (type_of_field == "Triangle"){
        key = 2;
    }
    if (type_of_field == "Hex"){
        key = 3;
    }
    bool charged = false;

    //cout << "before apple initialization" << endl;

    Apple ap(field[0], step/4, {255, 1, 1, 255}, {1, 1, 1, 255}, 1, true);

    //cout << "after apple initialization" << endl;

    if (type_of_field == "Hex"){
        ap.radius = step/3;
    }

    //cout << "Window is starting now" << endl;

    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (type_of_field == "Square"){
                if (event.type == Event::KeyPressed) {
                    //cout << event.key.code << endl;
                    if ((event.key.code == 22)&&(key!=1)) {
                        key = 0; //w
                    }
                    if ((event.key.code == 18)&&(key!=0)) {
                        key = 1; //s
                    }
                    if ((event.key.code == 0)&&(key!=3)) {
                        key = 2; //a
                    }
                    if ((event.key.code == 3)&&(key!=2)) {
                        key = 3; //d
                    }
                }
            }
            if (type_of_field == "Triangle"){
                if (event.type == Event::KeyPressed) {
                    //cout << event.key.code << endl;
                    if (event.key.code == 22) {
                        key = 0; //w
                    }
                    if (event.key.code == 0) {
                        key = 1; //a
                    }
                    if (event.key.code == 3) {
                        key = 2; //d
                    }
                }
            }
            if (type_of_field == "Hex"){
                if (event.type == Event::KeyPressed) {
                    //cout << event.key.code << endl;
                    if (event.key.code == 22) {
                        key = 0; //w
                    }
                    if (event.key.code == 4) {
                        key = 1; //e
                    }
                    if (event.key.code == 0) {
                        key = 2; //a
                    }
                    if (event.key.code == 3) {
                        key = 3; //d
                    }
                    if (event.key.code == 25) {
                        key = 4; //z
                    }
                    if (event.key.code == 23) {
                        key = 5; //x
                    }
                }
            }
        }

        window.clear(Color::White);

        for (int i=0; i<field.size(); i++){
            window.draw(draw_tile(field[i]));
        }

        for (int i=0;i<s.vert.size(); i++){
            if (s.vert[i] == ap.tile.center){
                ap.is_eaten = true;
                cout << "An apple has been eaten!" << endl;
                charged = true;
            }
        }

        s.time += Snake_speed;

        if (Difficulty == "Hard"){
            Snake_speed += 1e-7;
        }

        if (s.time >= 1){
            //cout << "Snake Head is now at: " << cur.center.x << "/" << cur.center.y << endl;

            //cout << "cur = *(cur.close[key]);" << endl;

            cur = *(cur.close[key]);

            //cout << cur.close[0]->center.x << " %" << cur.close[0]->center.y << " %" << endl;
            s.vert.push_back(cur.center);
            s.time = 0;
            if (!charged){
                s.vert.erase(s.vert.begin());
            }else{
                charged = false;
                if (Difficulty == "Hard"){
                    Snake_speed += 1e-3;
                }
            }
        }

        if (ap.is_eaten){
            int k = 0;
            ap.is_eaten = false;
            bool found = false;
            int rnd = rand()%(field.size());
            Vector2f random_tile;
            while(!found){
                k++;
                rnd = (rnd + 1)%(field.size());
                random_tile = field[rnd].center;
                found = true;
                for (int i = 0; i < s.vert.size(); i++) {
                    if(random_tile == s.vert[i]){
                        found = false;
                    }
                }
                if (k>1e6){
                    win_screen(s.vert.size());
                    return 0;
                }
            }
            ap.tile = field[rnd];
        }

        if (!ap.is_eaten){
            Vector2f offset;
            offset = {ap.radius, ap.radius};
            CircleShape circle;
            circle.setRadius(ap.radius);
            circle.setFillColor(ap.fill);
            circle.setOutlineColor(ap.countour);
            circle.setOutlineThickness(ap.thick);
            circle.setPosition(ap.tile.center - offset);
            window.draw(circle);
            //cout << ap.tile.center.x << ' ' << ap.tile.center.y << endl;
        }

        if (check_if_dead(s)){
            cout << "You have died(" << endl;
            if (Difficulty=="Normal"){
                cout << "Your score is: " << s.vert.size();
                window.close();
            }

            if (Difficulty=="Hard"){
                cout << "Your score is: " << s.vert.size();
                window.close();
            }
        }

        window.draw(draw_snake_head(s, step, s.time));

        for (int i=1; i<s.vert.size()-2; i++){
            window.draw(draw_snake(s, i, step));
        }

        window.draw(draw_snake_tail(s, step, s.time));
        window.display();

    }

    RenderWindow post(VideoMode(0, 0), "End screen!", Style::Default, settings);
    post.clear(Color::White);
    while(post.isOpen()) {
        Event event;
        while (post.pollEvent(event)) {
            if (event.type == Event::Closed) {
                post.close();
            }
        }
    }

    return 0;
}