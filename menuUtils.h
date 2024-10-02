#include <iostream>

void printStartText(void){
    std::cout << std::endl;
    std::cout << "Welcome to MineCraft MazeRunner!" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void printMainMenu(void){
    std::cout << std::endl;
    std::cout << "------------- MAIN MENU -------------" << std::endl;
    std::cout << "1) Generate Maze" << std::endl;
    std::cout << "2) Build Maze in MineCraft" << std::endl;
    std::cout << "3) Solve Maze" << std::endl;
    std::cout << "4) Show Team Information" << std::endl;
    std::cout << "5) Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printGenerateMazeMenu(void){
    std::cout << std::endl;
    std::cout << "------------- GENERATE MAZE -------------" << std::endl;
    std::cout << "1) Read Maze from terminal" << std::endl;
    std::cout << "2) Generate Random Maze" << std::endl;
    std::cout << "3) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printSolveMazeMenu(void){
    std::cout << std::endl;
    std::cout << "------------- SOLVE MAZE -------------" << std::endl;
    std::cout << "1) Solve Manually" << std::endl;
    std::cout << "2) Show Escape Route" << std::endl;
    std::cout << "3) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}


void printTeamInfo(void){
    std::cout << std::endl;
    std::cout << "Team members:" << std::endl;
    std::cout << "\t [1] Thomas Zooeff (s3721123@student.rmit.edu.au)" << std::endl;
    std::cout << "\t [2] Jatanjeet Singh (s4007591@student.rmit.edu.au)" << std::endl;
    std::cout << "\t [3] Sukhman Singh Rai (s4008880@student.rmit.edu.au)" << std::endl;
    std::cout << "\t [4] Dhruv Kathuria (s3993592@student.rmit.edu.au)" << std::endl;
    std::cout << std::endl;
}

void printAskBaspoint(void){
    std::cout << "Enter the baspoint of the maze:" << std::endl;
}

void printAskDimensions(void){
    std::cout << "Enter the length and width of the maze:" << std::endl;
}

void printAskMaze(void){
    std::cout << "Enter the structure of the maze:" << std:: endl;
}

void printExitMessage(void){
    std::cout << std::endl;
    std::cout << "The End!" << std::endl;
    std::cout << std::endl;
}

void printMainMenuError(void) {
    std::cout << std::endl;
    std::cout << "Input Error: Enter a number between 1 and 5" << std::endl;
    std::cout << std::endl;
}

void printSubMenuError(void) {
    std::cout << std::endl;
    std::cout << "Input Error: Enter a number between 1 and 3" << std::endl;
    std::cout << std::endl;
}

void printSolveMazeError(void){
    std::cout << "initialise player using solve manually." << std::endl;
}

void printMazeReady(void){
    std::cout << "Maze ready to solve ..." << std::endl;
}