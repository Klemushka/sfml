#include <SFML/Graphics.hpp>
#include <iostream>
#include "map.h"
#include "view.h"

using namespace sf;


class Isaac {
private:
float x, y=0;
public:
 float  w, h, dx, dy, speed = 0;
 int dir = 0;
 String File;
 Image image;
 Texture texture;
 Sprite sprite;

Isaac(String F, float X, float Y, float W, float H){
 File = F;
 w = W; h = H;
 image.loadFromFile("sprites/" + File);
 texture.loadFromFile("sprites/" + File);
 sprite.setTexture(texture);
 x = X; y = Y;
 sprite.setTextureRect(IntRect(0, 79, w, h));
 sprite.scale(3,3);
}

void update(float time)
{
switch (dir)
{
case 0: dx = speed; dy = 0; break;
case 1: dx = -speed; dy = 0; break;
case 2: dy = speed; dx = 0; break;
case 3: dy = -speed; dx = 0; break;
}

x +=dx*time;
y +=dy*time;

speed = 0;
sprite.setPosition(x,y);

interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
}

void interactionWithMap()//ф-ция взаимодействия с картой
	{

			for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j<(x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy>0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy<0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx>0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}

				if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
					x = 300; y = 300;//какое то действие... например телепортация героя
					TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
				}
			}
	}

float getplayercoordinateX(){	//этим методом будем забирать координату Х
		return x;
	}
	float getplayercoordinateY(){	//этим методом будем забирать координату Y
		return y;
	}

};


int main()
{



	RenderWindow window(VideoMode(640, 480), "Isaac.");

	view.reset(sf::FloatRect(0, 0, 640, 480));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации.

    Image map_image;//объект изображения для карты
	map_image.loadFromFile("sprites/map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

    Isaac me("Isaac.png", 100, 79, 19, 15);

	float CurrentFrame = 0;
	Clock clock;

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 700;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}



         if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
             me.dir = 1; me.speed = 0.1;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
             CurrentFrame += 0.005*time;
             if (CurrentFrame > 10) CurrentFrame -= 10;
             me.sprite.setTextureRect(IntRect(19 * int(CurrentFrame)+19, 122, -19, 15)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
             getplayercoordinateforview(me.getplayercoordinateX(), me.getplayercoordinateY());
 }

         if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
             me.dir = 0; me.speed = 0.1;//направление вправо, см выше
             CurrentFrame += 0.005*time;
             if (CurrentFrame > 10) CurrentFrame -= 10;
             me.sprite.setTextureRect(IntRect(19 * int(CurrentFrame), 122, 19, 15)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
             getplayercoordinateforview(me.getplayercoordinateX(), me.getplayercoordinateY());
 }

         if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
             me.dir = 3; me.speed = 0.1;//направление вниз, см выше
             CurrentFrame += 0.005*time;
             if (CurrentFrame > 10) CurrentFrame -= 10;
             me.sprite.setTextureRect(IntRect(19 * int(CurrentFrame)+19, 79, -19, 15)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
             getplayercoordinateforview(me.getplayercoordinateX(), me.getplayercoordinateY());
 }

         if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //если нажата клавиша стрелка влево или англ буква А
             me.dir = 2; me.speed = 0.1;//направление вверх, см выше
             CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
             if (CurrentFrame > 10) CurrentFrame -= 10; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
             me.sprite.setTextureRect(IntRect(19 * int(CurrentFrame), 79, 19, 15)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
             getplayercoordinateforview(me.getplayercoordinateX(), me.getplayercoordinateY());
 }

        if ((Keyboard::isKeyPressed(Keyboard::Escape))) {
            window.close();
 }

if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		me.update(time);


		window.setView(view);//"оживляем" камеру в окне sfml

		window.clear(Color::White);
/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
			if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
			if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик


			s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

			window.draw(s_map);//рисуем квадратики на экран
		}
		window.draw(me.sprite);
		window.display();
	}

	return 0;
}
