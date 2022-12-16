#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include<cmath>
#include<glut.h>
#include <iostream>


double rotate_y = 0; //вращение стрелочками
double rotate_x = 0;

void Light() //функция для регулировки освещения
{
	glEnable(GL_LIGHT0); //нулевой источник света
	glEnable(GL_LIGHTING); // вкл расчет освещения
	glEnable(GL_DEPTH_TEST); //проверка на глубину 
	GLfloat pos[] = { -1.0, 1.0, 1.0, 1.0 }; //меняя значение тут, можно получить различное направление 
	glLightfv(GL_LIGHT0, GL_POSITION, pos); //определение источников света (номер, параметры, направление)
}


void specialKeys(int key, int x, int y) { // отклонение по стрелкам на 5 градусов

	if (key == GLUT_KEY_RIGHT)
		rotate_y += 5;
	else if (key == GLUT_KEY_LEFT)
		rotate_y -= 5;
	else if (key == GLUT_KEY_UP)
		rotate_x += 5;
	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 5;
	glutPostRedisplay(); //обновление дисплея
}

GLuint groundTex;
GLuint LoadTexture(const char* filename){ //Натянуть текстуру на полигон
	GLuint texture;
	int width, height;
	unsigned char* data;
	FILE* file;
	file = fopen(filename, "rb");
	if (file == NULL) return 0; //Не считывать несуществующий файл
	width = 256; //Размер изображения в пикселях
	height = 256;
	data = (unsigned char*)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file); //возвращает количество прочитанных элементов
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R; 
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	return texture;

}

void grass() {//Отображение поверхности с текстурой 
	groundTex = LoadTexture("C:\\Users\\bfoyb\\OneDrive\\Desktop\\KG_KR\\grass.bmp");

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glEnable(GL_POLYGON_OFFSET_FILL);

	glColor3f(0.25f, 1.0f, 0.0f);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.9, 0.9, 0.0);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.9, -0.9, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.9, -0.9, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.9, 0.9, 0.0f);
	glEnd();


	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

double zs[10], zs2[10], xs[10];

void getnewz() // перемещает по оси z (вниз)
{
	for (int i = 0; i < 10; i++)
	{
		if (zs[i] > -0.33)
			zs[i] -= 0.02;
		else break;
	}
}
void getnewz2() // перемещает по оси z(вверх)
{
	for (int i = 0; i < 10; i++)
			zs2[i] += 0.0025;
}
void getnewx() // перемещает по оси У
{
	for (int i = 0; i < 10; i++)
			xs[i] -= 0.010;

}


void drawApple(double z) //отрисовка яблока
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslated(0.1, 0.1, z);
	glutSolidSphere(0.03, 50, 50);
	glPopMatrix();
}

void drawSmoke(double x, double z) //отрисовка яблока
{
	glPushMatrix();
	glColor3f(0.7f, 0.7f, 0.7f);
	glTranslated(x, 0.1, z);
	glutWireSphere(0.045, 50, 10);
	glPopMatrix();
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float x = 0.0; //центр
	float y = 0.0;
	float z = 0.0;
	glLoadIdentity();
	// Rotate when user changes rotate_x and rotate_y
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);

	glRotatef(90, -1.0, 0.0, 0.0); //повернули картинку, чтобы не верх дном все рисовалось
	glTranslatef(x, y, z - 0.2);
	grass();

	glTranslatef(x + 0.1, y + 0.1, z);
	glColor3f(0.5f, 0.3f, 0.25f); //коричневый
	glTranslatef(x - 0.65, y - 0.5, z); //перенос конуса)
	glutSolidCone(0.1f, 0.5f, 100, 100); //конус (база, высота, грани, полигоны)

	glColor3f(0.25f, 1.0f, 0.0f); //установка зеленого цвета
	glTranslatef(x, y, z + 0.5);
	glutSolidSphere(0.255, 50, 50);
	glTranslatef(x - 0.1, y - 0.1, z);

	glColor3f(0.5f, 0.3f, 0.25f); //коричневый
	glTranslatef(x + 0.6, y, z - 0.5); //перенос 2 конуса)
	glutSolidCone(0.05f, 0.5f, 100, 100); //конус (база, высота, грани, полигоны)

	glColor3f(0.25f, 1.0f, 0.0f); //установка зеленого цвета
	glTranslatef(x, y, z + 0.5);
	glutSolidSphere(0.2, 50, 50);

	glColor3f(0.5f, 0.3f, 0.25f); //коричневый
	glTranslatef(x - 0.6, y + 0.6, z - 0.5); //перенос 3 конуса)
	glutSolidCone(0.035f, 0.7f, 100, 100); //конус (база, высота, грани, полигоны)

	glColor3f(0.25f, 1.0f, 0.0f); //установка зеленого цвета
	glTranslatef(x, y, z + 0.6);
	glutSolidSphere(0.15, 50, 50);

	glTranslatef(x + 0.65, y - 0.1, z - 0.6); //отцентровка координат после деревьев :)

	glTranslatef(x-0.1, y-0.1, z);

	glColor3f(184.0/255, 115.0/255, 51.0/255); //почти черный
	glTranslatef(x + 0.4, y + 0.4, z + 0.3001); //дом относительно центра (z поднято, ибо указывается центр куба)
	glutSolidCube(0.6);

	//glColor3f(77.0 / 255, 34.0 / 255, 14.0 / 255); //коричневый цвет крышы
	glColor3f(60.0 / 255, 25.0 / 255, 10.0 / 255); //цвет крыши
	glBegin(GL_POLYGON); //2Д крыша относительно ЦЕНТРА КУБА
	glVertex3f(-0.4f, 0.4f, 0.3); 
	glVertex3f(0.4f, 0.4f, 0.3);
	glVertex3f(0, 0, 0.6f);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-0.4f, -0.4f, 0.3); 
	glVertex3f(0.4f, -0.4f, 0.3);
	glVertex3f(0, 0, 0.6f);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-0.4f, -0.4f, 0.3);
	glVertex3f(-0.4f, 0.4f, 0.3);
	glVertex3f(0, 0, 0.6f);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(0.4f, -0.4f, 0.3); 
	glVertex3f(0.4f, 0.4f, 0.3);
	glVertex3f(0, 0, 0.6f);
	glEnd();

	glBegin(GL_QUADS); //низ крыши, чтобы не просвечивало
	glVertex3f(0.4f, 0.4f, 0.3);
	glVertex3f(0.4f, -0.4f, 0.3);
	glVertex3f(-0.4f, -0.4f, 0.3);
	glVertex3f(-0.4f, 0.4f, 0.3);
	glEnd();

	glColor3f(150.0 / 255, 35.0 / 255, 33.0 / 255); //красно-коричневый цвет трубы

	glTranslatef(x + 0.2, y + 0.2, z + 0.5); //штука для трубы. x=y=0.6, z= 0.8
	glutSolidCube(0.1);
	glTranslatef(x, y, z-0.1); //штука для трубы. x=y=0.6, z= 0.7
	glutSolidCube(0.1);

	glTranslatef(x, y, z + 0.2);  //ОЧЕНЬ много дыма
	for (int i = 0; i < 10; i++) //для движения анимированного объекта
	{
		drawSmoke(xs[i], zs2[i]);
	}
	getnewz2();
	getnewx();

	glTranslatef(x-0.05, y, z + 0.05);
	for (int i = 0; i < 10; i++) //для движения анимированного объекта
	{
		drawSmoke(xs[i], zs2[i]);
	}
	getnewz2();
	getnewx();

	glTranslatef(x, y-0.05, z - 0.02);
	for (int i = 0; i < 10; i++) //для движения анимированного объекта
	{
		drawSmoke(xs[i], zs2[i]);
	}
	getnewz2();
	getnewx();
	glTranslatef(x, y - 0.01, z + 0.02);
	for (int i = 0; i < 10; i++) //для движения анимированного объекта
	{
		drawSmoke(xs[i], zs2[i]);
	}
	getnewz2();
	getnewx();

	glTranslatef(x+0.02, y, z + 0.02);
	for (int i = 0; i < 10; i++) //для движения анимированного объекта
	{
		drawSmoke(xs[i], zs2[i]);
	}
	getnewz2();
	getnewx();
	glTranslatef(x + 0.1, y + 0.1, z);
	glTranslatef(x-0.2, y + 0.01, z - 0.02); //отцентровка после дыма и прочего
	glTranslatef(x, y + 0.05, z+0.02);
	glTranslatef(x, y, z - 0.2);
	glTranslatef(x + 0.15, y, z - 0.05);
	glTranslatef(x-0.6, y-0.6, z - 0.7);

	glColor3f(1, 0, 0); //красный для яблок

	//ЯБЛОКИИИИИ (лежат)
	//glTranslatef(x, y, z - 0.02);
	glTranslatef(x+0.2, y-0.6, z+0.015); //0.2, -0.6
	glutSolidSphere(0.030, 50, 50);
	glTranslatef(x-0.6, y+0.1, z); //-0.4,  -0.5
	glutSolidSphere(0.035, 50, 50);
	glTranslatef(x, y+0.2, z); //-0.4, -0.3
	glutSolidSphere(0.030, 50, 50);
	glTranslatef(x+0.2, y, z); //-0.2, -0.3
	glutSolidSphere(0.035, 50, 50);
	glTranslatef(x-0.55, y, z); //-0.8, -0.3
	glutSolidSphere(0.04, 50, 50);
	glTranslatef(x, y+0.5, z); //-0.8, 0.2
	glutSolidSphere(0.035, 50, 50);
	glTranslatef(x+0.15, y-0.4, z); //-0.6, -0.2
	glutSolidSphere(0.035, 50, 50);

	glTranslatef(x + 0.6, y + 0.2, z - 0.035); //отцентровка х, у и z
	glTranslatef(x - 0.1, y - 0.1, z);

	glColor3f(30.0 / 255, 7.0 / 255, 6.80 / 255); //красно-коричневый цвет двери
	glTranslatef(x + 0.3, y+0.63, z+0.075);
	glutSolidCube(0.15);
	glTranslatef(x, y, z + 0.075);
	glutSolidCube(0.15);

	glColor3f(1, 1, 0); //желтый для ручки
	glTranslatef(x-0.05, y+0.07, z - 0.06);
	glutSolidSphere(0.015, 50, 50);

	glColor3f(0.7, 0.7, 1); //голубой для окон
	glTranslatef(x + 0.05, y - 0.07, z + 0.06); //вернула на смещение двери
	glTranslatef(x + 0.3, y, z + 0.27);
	glutSolidCube(0.15);
	glTranslatef(x, y-0.465, z);
	glutSolidCube(0.15);

	glTranslatef(x + 0.1, y + 0.1, z);
	glTranslatef(x-0.5, y - 0.165, z-0.42);//отцентровка
	glTranslatef(x-0.6, y-0.27, z + 0.37);
	for (int i = 0; i < 10; i++) //для движения анимированного объекта
	{
		drawApple(zs[i]);
	}
	getnewz();
	glTranslatef(x + 0.62, y + 0.3, z - 0.42); //отцентровка



	glFlush();
	glutSwapBuffers();
}
void Initialize()
{
	glClearColor(0.9, 0.85, 1, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 0, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Поставьте 35 в честь нового года :)");
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(Display);
	glutSpecialFunc(specialKeys);
	Initialize();
	Light();
	glutMainLoop();
	return 0;
}