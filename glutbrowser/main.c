﻿#define _CRT_SECURE_NO_WARNINGS

// For now we're just going to keep everything in a single TU
#include "cssparser.h"
#include "gluthelpers.h"
#include "htmlparser.h"
#include "jsparser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <GL/freeglut.h>

// TODO 1-char tag names are broken
// TODO spaces in "Content" are broken
const char* test_html = "<head></head><body><h1>This is a heading\nThis is a second line</h1></body>";

unsigned int modstate;
int cur_key = -1;
int cur_skey = -1;
int win_width, win_height;

void draw_text(int x, int y, const char* str)
{
	int line_height = 24;
	glRasterPos2i(x, y);
	while (*str) {
		if (*str == '\n') {
			y -= line_height;
			glRasterPos2i(x, y);
			str++;
			continue;
		}
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str++);
	}
}

void reshape(int x, int y)
{
	win_width = x;
	win_height = y;
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, x, 0, y, -1, 1);
}

void keypress(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);

	modstate = glutGetModifiers();
	cur_key = key;
	glutPostRedisplay();
}

void keyrelease(unsigned char key, int x, int y)
{
	cur_key = -1;
	glutPostRedisplay();
}

void skeypress(int key, int x, int y)
{
	cur_skey = key;
	glutPostRedisplay();
}

void skeyrelease(int key, int x, int y)
{
	cur_skey = -1;
	glutPostRedisplay();
}

// here temporarily
char buf_content[128] = "";

void display(void)
{
	char str[256];

	glClear(GL_COLOR_BUFFER_BIT);

	strcpy(str, "Key:");
	if (cur_key > 0) {
		if (isprint(cur_key)) {
			sprintf(str + 4, " '%c'", cur_key);
		} else {
			sprintf(str + 4, " 0x%02x", cur_key);
		}

		if (modstate & GLUT_ACTIVE_SHIFT) {
			strcat(str, "  shift");
		} if (modstate & GLUT_ACTIVE_CTRL) {
			strcat(str, "  ctrl");
		} if (modstate & GLUT_ACTIVE_ALT) {
			strcat(str, "  alt");
		} if (modstate & GLUT_ACTIVE_SUPER) {
			strcat(str, "  super");
		}
	}
	draw_text(win_width / 3, 2 * win_height / 3, str);

	strcpy(str, "Special key: ");
	if (cur_skey > 0) {
		strcat(str, skeyname(cur_skey));
	}
	draw_text(win_width / 3, win_height / 3, str);

	draw_text(0, win_height - 24, buf_content);

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(1600, 900);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("glutbrowser");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyrelease);
	glutSpecialFunc(skeypress);
	glutSpecialUpFunc(skeyrelease);

	parseHTML(test_html, buf_content);
	glutMainLoop();
	return 0;
}
