# mine_sweeper
A C++ project to design the best strategy for minesweeper game
​
# 1.Review more generally about the whole project to generate a readme
1.the resource.h define some macro which will be used in the other function

2.the .rc file include the resource.h file and define more macros here ,the Cbutton.rc file defines all the graphical and text resources used by the application, making it easier to manage and reference them in the project.so it's only used to make it easier to refer them in the project

3.the Frm.cpp defines the windows size and the menu size and something like it

4.and finally —— it's our CbuttonView.cpp file, this is the main file of the application, let's break down the whole file structure

(1)Message Map:The message map allows the framework to route messages sent to instances of CCbuttonView to the appropriate member functions for handling. It establishes a connection between Windows messages or user commands and the corresponding member functions in the class.

(2)Automatic mine sweeper strategy func:this part input the current GPanel and output the next position (x,y) to strike

(3)RefreshGPanel and Machine part : this part get the (x,y) and refresh the GPanel, determine whether the game is over.

(4)initiate the GPanel and draw the base of it: the Construtor of the class and the OnMenuNew func

(5)strange part ??? I don't really understand yet:like draw,prepare ...  the previous work's author try to rewrite some func in the afxwin.h but something haven't done yet

(6)OnCreate part: create the id of the 20*20 pixel button

(7)Left button and Right button part : the left click only use the base func enough, and the right click let the client reveal all the mine which is a cheating way

(8)Ondraw part: this part use the pen and brush object, and draw the line of the row and col ,draw the block and number , if the game end it also draw the mine 

(9)OnCommand func part: a very important func, it get the index of the click pixel as input, and calculate the clickcount here ,and get the ifwin flag from the RefreshGPanel func then MessageBox to deliver the win or loss infomation with refresh the whole panel . Also , it show the GPanel again. Fianlly, it call the base OnCommand func to execute something must be execute

(10)On Machine part (I see it as a entry of the whole project): it calls the machine func that define the strategy to choose the index of the next pixel chosen by automatically

# 2. How to run this project with GUI？
1. I have a classmate who had already run this GUI project successfully and our TA has run too,so I will consult them if necessary.

(1) our classmate's video show: (I just borrowed this video from him)

【中山大学本科生C++大作业】一个很简陋的自动扫雷QWQ_哔哩哔哩_bilibili

2.Just get the project from my github and run the .sln file is ok

3.Especially, this is a MFC application. I highly recommend you to download the Visual Studio 2022 and install the necessary MFC tool or you will face the running problem. But this maybe the only problem and just enjoy the game!

​
