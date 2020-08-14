#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

using namespace std;

#define EPS 1e-9

struct point {
	double x, y;

	point() {x = y = 0.0;}
	point(double _x, double _y) : x(_x), y(_y) {}

	point operator +(point other) const {
		return point(x + other.x, y + other.y);
	}

	point operator -(point other) const {
		return point(x - other.x, y - other.y);
	}

	point operator *(double k) const {
		return point(x*k, y*k);
	}
};

double dist(point p1, point p2){
	return hypot(p1.x - p2.x, p1.y - p2.y);
}

double cross(point p1, point p2){
	return p1.x*p2.y - p1.y*p2.x;
}

struct circle {
	point c;
	double r;

	circle(){ c = point(); r = 0; }
	circle(point _c, double _r) : c(_c), r(_r) {}

	bool contains(point p){
		return dist(c, p) <= r + EPS;
	}
};

circle circumcircle(point a, point b, point c){
	circle ans;
	point u = point((b-a).y, -(b-a).x);
	point v = point((c-a).y, -(c-a).x);
	point n = (c-b)*0.5;
	double t = cross(u,n)/cross(v,u);
	ans.c = ((a+c)*0.5) + (v*t);
	ans.r = dist(ans.c, a);
	return ans;
}

void drawAll(vector<point> &p, circle &C, sf::RenderWindow &window,
	bool fast,
	int i, int j = -1, int k = -1){

	printf("%d %d %d\n", i, j, k);
	printf("%lf\n", C.r);

	sf::Clock clock;

	window.clear(sf::Color::White);

	sf::CircleShape mainCircle(C.r);
	mainCircle.setFillColor(sf::Color(125, 125, 125));
	mainCircle.setPosition(C.c.x - C.r, C.c.y - C.r);

	window.draw(mainCircle);

	for(int ind = 0; ind < p.size(); ind++){
		sf::CircleShape shape(3.f);
		shape.setPosition(p[ind].x - 3.f, p[ind].y - 3.f);
		if(ind == i){
			shape.setFillColor(sf::Color(250, 0, 0));
		} else if (ind == j){
			shape.setFillColor(sf::Color(0, 250, 0));
		} else if (ind == k){
			shape.setFillColor(sf::Color(0, 0, 250));
		} else {
			shape.setFillColor(sf::Color(0, 0, 0));
		}
		window.draw(shape);
	}

	sf::Time elapsed = clock.getElapsedTime();

	double timetowait = 1.5f;
	if(fast) timetowait = .7f;

	window.display();

	while(elapsed.asSeconds() < timetowait){
		elapsed = clock.getElapsedTime();

		sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

	}

}

circle minimumCircle(vector<point> p, sf::RenderWindow &window){

	random_shuffle(p.begin(), p.end());

	circle C = circle(p[0], 0.0);

	for(int i = 0; i < p.size(); i++){
		if(C.contains(p[i])){
			drawAll(p, C, window, true, i);
			continue;
		}
		drawAll(p, C, window, false, i);
		C = circle(p[i], 0.0);
		drawAll(p, C, window, false, i);
		for(int j = 0; j < i; j++){
			if(C.contains(p[j])){
				drawAll(p, C, window, true, i, j);
				continue;
			}
			drawAll(p, C, window, false, i, j);
			C = circle((p[j] + p[i])*0.5, 0.5*dist(p[j], p[i]));
			drawAll(p, C, window, false, i, j);
			for(int k = 0; k < j; k++){
				if(C.contains(p[k])) {
					drawAll(p, C, window, true, i, j, k);
					continue;
				}
				drawAll(p, C, window, false, i, j, k);
				C = circumcircle(p[i], p[j], p[k]);		
				drawAll(p, C, window, false, i, j, k);
			}
		}
	}

	return C;
}


int main(){
	int n;
	cin >> n;

	vector<point> p;
	for(int i = 0; i < n; i++){
		double x, y;
		cin >> x >> y;
		p.push_back(point(x,y));
	}

	sf::RenderWindow window(sf::VideoMode(500, 500), "Circle");

	circle result;

	sf::Event event;

	result = minimumCircle(p, window);

	while(window.isOpen()){
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed) window.close();
		}
	}

	cout.precision(2);
    cout << fixed << result.c.x << " " << result.c.y << " " << result.r << " " << 4 * 2 * 3.14 * result.r << "\n";
	return 0;
}

