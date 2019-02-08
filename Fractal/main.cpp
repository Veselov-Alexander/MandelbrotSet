#include <iostream>
#include "program.h"

int window_width = 500, window_height = 400;

const int INITIAL_MAX_ITERATIONS = 256;

void process_arguments(int argc, char **argv)
{
    if (argc == 3)
    {       
        window_width = atoi(argv[1]);
        window_height = atoi(argv[2]);
    }
}

int main(int argc, char **argv)
{
    process_arguments(argc, argv);

    initialize(window_width, window_height, INITIAL_MAX_ITERATIONS);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Mandelbrot set.");
    update_title();
    glClearColor(0, 0, 0, 1.0);
    glOrtho(-window_width / 2, window_width / 2, window_height / 2, -window_height / 2, -50, 50);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}
