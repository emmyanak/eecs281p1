//
//  main.cpp
//  281project1
//
//  Created by Emma Yanakiev on 5/4/16.
//  
//

#include <iostream>
#include <getopt.h>
#include <sstream>
#include <deque>
#include "project1location.h"
#include <vector>
// ...

using namespace std; 




int main(int argc, char **argv) {



  string line;
  int row = 0;
  int col = 0;
  int state = 0;
  

 
  int indx = 0;

  static struct option longopts[] = 
    {
       {"stack", no_argument, NULL, 's'},
       {"queue", no_argument, NULL, 'q'},
       {"output", required_argument, NULL, 'o'},
       {"help", no_argument, NULL, 'h'},
       {NULL, 0, NULL, 0}
    };

    
    int c;
    
    bool stack_flag = false;
    bool q_flag = false;
    char output_mode = 'M'; //output mode is set to M
    while ((c = getopt_long(argc, argv, "sqo:h", longopts, &indx)) != -1)
    {
        switch(c) {
            case 's': stack_flag = true; break;
            case 'q': q_flag = true; break;
            case 'o':
                if (optarg == NULL)
                {
                    output_mode = 'M';
                }
                else
                {
                    output_mode = *optarg;
                }
                    break;
        
            case 'h': cout << "help msg" << "/n"; return 0; //help case
           
        }
    }
    
    if ((stack_flag == true) && (q_flag == true)) 
       {
           cerr << "user has selected both search modes" << "\n";
           exit(1);	
       }
    if ((stack_flag == false) && (q_flag == false)) 
       {
           cerr << "user has not indicated a search modes" << "\n";
           exit(1);	
       }
    
    //read the file header
    char input_type;
    int N, num_states;    
    cin >> input_type >> N >> num_states;
    
    Location location0;
    location0.visited = false;
    location0.point_type = '.';
    location0.direction = 'm';


    vector<vector<vector<Location> > > map(num_states, vector<vector<Location> >(N, vector<Location>(N, location0)));; //creat 3d vector
                                                                          
                                                                                     
    char unwanted = '\0';
    char new_value = '\0';
    char point_char = '\0';
    int startpoint_col = 0;
    int startpoint_row = 0;
    int startpoint_state = 0; //save the location of U, the starting point
    int R_state = 0;
    int R_row = 0;
    int R_col = 0;

   
    
    //IF LINE INPUT
    if (input_type == 'L')
    {
        string line;
        
        while (cin >> new_value) {
            if (new_value == '/') //if there is a comment line, skip it
            {
                getline(cin, line);
            }
            else
            {
                cin >> row >> unwanted >> col >> unwanted >> state >> unwanted >> point_char >> unwanted;
                if (point_char != '.' && point_char != '#' && point_char != '<' 
                    && point_char != '>' && point_char != 'U' && point_char != 'R') 
                {
		   cerr << "Invalid input character" << "\n";
                   exit(1); 
                }
                if (row >= N || row < 0 || col >= N || col < 0 || state >= num_states || state < 0)
                {
	           cerr << "Invalid state, row, or column value" << "\n";
                   exit(1); 
                }
              
                map[state][row][col].point_type = point_char; 
                if (point_char == 'U') //store the starting point 
                {
                    startpoint_col = col;
                    startpoint_row = row;
                    startpoint_state = state;
	            map[state][row][col].visited = true;;
                }//close if starting point
            }//if else
        }//while cin >> new_value
    }//if it's a line input

    //IF MAP INPUT
    else //if (input_type == 'M') //if it's a map input
    { 
        bool comment = false;
        row = 0;
        col = 0;
        state = 0; 
        
        while (cin >> new_value)
        {
             
      	    point_char = new_value;
     
            if (point_char == '/') //if there is a comment, get line and exit loop
            { 
                    getline(cin, line);
            }
            else
            {
            while (row < N){ 
                if (point_char == '/') //if there is a comment, get line and exit loop
                {
                   getline(cin, line);
                    comment = true; 
                }
                if (!comment) {
                for (int i = 0; i < N; i++)
                {
                    
                    col = i;
                     if (point_char != '.' && point_char != '#' && point_char != '<' 
                    && point_char != '>' && point_char != 'U' && point_char != 'R') 
                    {
		       cerr << "Invalid input character" << "\n";
                       exit(1); 
                    }
                    map[state][row][col].point_type = point_char; //add point to map
                    if (point_char == 'U') //look for starting point
                    {
                        startpoint_col = col;
                        startpoint_row = row;
                        startpoint_state = state;
			map[state][row][col].visited = true;
                    }//close if starting point           
                    if (i < N-1)
                    {
                    cin >> point_char;
                    }
                }//loop through this line           
               row = row+1; //increment row 
               } //at the end of every line 
               
               comment = false; 
               cin >> point_char;
                
            }//loop through the full state
            state = state+1;
            row = 0; //get row back to zero when entering new state
          }
        }//while reading in file
       
    }//if input is a map

//SEARCH:


//create the deque of type Path 
deque<Path> journey;

int cur_state = startpoint_state;
int cur_row = startpoint_row;
int cur_col = startpoint_col;
Path path;
Path current_path; 
bool route_found = false; 

//put initial point U in the deque
path.state = startpoint_state;
path.row = startpoint_row;
path.col = startpoint_col;
journey.push_back(path);


while (!route_found && !journey.empty()) 
{

 if (stack_flag == true) //stack mode
 {
    current_path = journey.back();
    journey.pop_back();
 }
  else //queue mode
  {
     current_path = journey.front();
     journey.pop_front();
  }

  cur_state = current_path.state;
  cur_row = current_path.row; 
  cur_col = current_path.col; 

if (map[cur_state][cur_row][cur_col].point_type == '<') //downstate flight
{
  if(cur_state-1 >= 0)
  {
     if (map[cur_state-1][cur_row][cur_col].point_type != '#'
     && map[cur_state-1][cur_row][cur_col].visited == false)
     {
       map[cur_state-1][cur_row][cur_col].direction = 'd'; 
       if (map[cur_state-1][cur_row][cur_col].point_type == 'R')
       { 
           route_found = true;
	   R_state = cur_state - 1; R_col = cur_col; R_row = cur_row;
       }
       else
       {
         map[cur_state-1][cur_row][cur_col].visited = true; 
        
         path.row = cur_row;
         path.state = cur_state-1;
         path.col = cur_col;
         journey.push_back(path);
       }
     }
   }
}
else if (map[cur_state][cur_row][cur_col].point_type == '>')
{
  if(cur_state+1 < num_states)
  {
     if (map[cur_state+1][cur_row][cur_col].point_type != '#'
     && (map[cur_state+1][cur_row][cur_col].visited == false))
     {
       map[cur_state+1][cur_row][cur_col].direction = 'u';        
       if (map[cur_state+1][cur_row][cur_col].point_type == 'R')
       { 
           route_found = true;
	   R_state = cur_state + 1; R_col = cur_col; R_row = cur_row;
       }
       else
       {
         map[cur_state+1][cur_row][cur_col].visited = true; 
         path.row = cur_row;
         path.state = cur_state+1;
         path.col = cur_col;
         journey.push_back(path);
       }
     }
   }
}
else
{ 
 if(cur_row-1 >= 0) //check if north point inbounds
 {
    if (map[cur_state][cur_row-1][cur_col].visited == false 
        && (map[cur_state][cur_row-1][cur_col].point_type != '#'))   
    {
       map[cur_state][cur_row-1][cur_col].direction = 'n'; 
       if (map[cur_state][cur_row-1][cur_col].point_type == 'R')
       { 
           route_found = true;
	   R_row = cur_row - 1; R_state = cur_state; R_col = cur_col;
       }
       else
       {
     	 map[cur_state][cur_row-1][cur_col].visited = true; 

         path.row = cur_row-1;
         path.state = cur_state;
         path.col = cur_col;
         journey.push_back(path); 
       }
    }
 }
  if(cur_col+1 < N) //check if east point inbounds
 {
    if (map[cur_state][cur_row][cur_col+1].visited == false 
        && (map[cur_state][cur_row][cur_col+1].point_type != '#'))   
    {
       map[cur_state][cur_row][cur_col+1].direction = 'e';
       if (map[cur_state][cur_row][cur_col+1].point_type == 'R')
       { 
           route_found = true;
	   R_col = cur_col + 1; R_state = cur_state; R_row = cur_row;
       }
       else
       {

     	 map[cur_state][cur_row][cur_col+1].visited = true; 
 
         path.row = cur_row;
         path.state = cur_state;
         path.col = cur_col+1;
         journey.push_back(path); 
       }
    }
 }
   if(cur_row+1 < N) //check if south point inbounds
 {
    if (map[cur_state][cur_row+1][cur_col].visited == false 
        && map[cur_state][cur_row+1][cur_col].point_type != '#')   
    {
       map[cur_state][cur_row+1][cur_col].direction = 's'; 
       if (map[cur_state][cur_row+1][cur_col].point_type == 'R')
       { 
           route_found = true;
	   R_row = cur_row + 1; R_state = cur_state; R_col = cur_col;
       }
       else
       {

     	 map[cur_state][cur_row+1][cur_col].visited = true; 
 
         path.row = cur_row+1;
         path.state = cur_state;
         path.col = cur_col;
         journey.push_back(path); 
       }
    }
 }
   if(cur_col-1 >=0) //check if west point inbounds
 {
    if (map[cur_state][cur_row][cur_col-1].visited == false 
        && (map[cur_state][cur_row][cur_col-1].point_type != '#'))   
    {
	map[cur_state][cur_row][cur_col-1].direction = 'w'; 
        if (map[cur_state][cur_row][cur_col-1].point_type == 'R')
       { 
           route_found = true;
	   R_col = cur_col - 1; R_state = cur_state; R_row = cur_row;
       }
       else
       {
     	 map[cur_state][cur_row][cur_col-1].visited = true; 
     	 
         path.row = cur_row;
         path.state = cur_state;
         path.col = cur_col-1;
         journey.push_back(path); 
       }
    }
  }
}

}//end of while

if (journey.empty() && !route_found) //if deque is empty before finding R 
{
    cout << N << "\n";
    cout << num_states << "\n";
    if (output_mode == 'L')
    {
     
        cout << "//path taken"; 
   }
   else //map output: reprint map 
   {
    //cout << "route not found " << endl; 
     for (int i = 0; i < num_states; ++i)
     {
       cout << "//state " << i << "\n";
        for (int j = 0; j < N; ++j)
        {
          for (int k = 0;k < N; ++k)
          {
           cout << map[i][j][k].point_type;
          }
           cout << "\n"; 
        }

     }  
   }
return 0; //exit 
}


//backtracking after reaching R

char current_char;
bool done_backtracking = false;

cur_state = R_state;
cur_row = R_row;
cur_col = R_col;

bool output_list = false; //if it is list output

if (output_mode == 'L')
{
  output_list = true;
}

 deque<Path>output_order; //create a deck to store output order
 Path point_coord; //initialze a path

if(route_found==true)
{
while (!done_backtracking) // go back until U is reached 
   {
     current_char = map[cur_state][cur_row][cur_col].direction; 
   
     if (current_char == 'n')
     {
        cur_row = cur_row+1; //move south 
	if (map[cur_state][cur_row][cur_col].point_type == 'U') 
        {
           done_backtracking = true; //check if beginning is reached
        }
        map[cur_state][cur_row][cur_col].point_type = 'n';
        if (output_list == true) 
        {
           point_coord.row = cur_row;
           point_coord.col = cur_col;
           point_coord.state = cur_state; 
           output_order.push_back(point_coord); 
        } 
          
     }
     else if (current_char == 'e')
     {
        cur_col = cur_col-1; //move west 
	if (map[cur_state][cur_row][cur_col].point_type == 'U') 
        {
           done_backtracking = true; //check if beginning is reached
        }
        map[cur_state][cur_row][cur_col].point_type = 'e';  
        if (output_list == true) 
        {
           point_coord.row = cur_row;
           point_coord.col = cur_col;
           point_coord.state = cur_state; 
           output_order.push_back(point_coord); 
        } 
     }
     else if (current_char == 's')
     {
        cur_row = cur_row-1; //move north
	if (map[cur_state][cur_row][cur_col].point_type == 'U') 
        {
           done_backtracking = true; //check if beginning is reached
        }
        map[cur_state][cur_row][cur_col].point_type = 's';  
        if (output_list == true) 
        {
           point_coord.row = cur_row;
           point_coord.col = cur_col;
           point_coord.state = cur_state; 
           output_order.push_back(point_coord); 
        } 
     }
     else if (current_char == 'w')
     {
        cur_col = cur_col+1; //move east 
	if (map[cur_state][cur_row][cur_col].point_type == 'U') 
        {
           done_backtracking = true; //check if beginning is reached
        }
        map[cur_state][cur_row][cur_col].point_type = 'w';  
        if (output_list == true) 
        {
           point_coord.row = cur_row;
           point_coord.col = cur_col;
           point_coord.state = cur_state; 
           output_order.push_back(point_coord); 
        } 
     }
     else if (current_char == 'u')
     {
        cur_state = cur_state-1; //move downstate 
if (map[cur_state][cur_row][cur_col].point_type == 'U') 
        done_backtracking = true; //check if beginning is reached
        map[cur_state][cur_row][cur_col].point_type = 'u'; 
        if (output_list == true) 
        {
           point_coord.row = cur_row;
           point_coord.col = cur_col;
           point_coord.state = cur_state; 
           output_order.push_back(point_coord); 
        }  
     }
     else //if (map[cur_state][cur_row][cur_col].direction == 'd')
     {
        cur_state = cur_state+1; //move upstate
	if (map[cur_state][cur_row][cur_col].point_type == 'U') 
        {
           done_backtracking = true; //check if beginning is reached
        }
        map[cur_state][cur_row][cur_col].point_type = 'd';  
        if (output_list == true) 
        {
           point_coord.row = cur_row;
           point_coord.col = cur_col;
           point_coord.state = cur_state; 
           output_order.push_back(point_coord); 
        } 
     } 
     
   }    //end while
}    // end if route found

//OUTPUT
  cout << N << "\n";
  cout << num_states << "\n";

ostringstream ss; //output stream 
if (output_mode == 'M') //map output requested 
{
  for (int i = 0; i < num_states; ++i)
  {
    ss << "//state " << i << "\n";
    for (int j = 0; j < N; ++j)
     {
       for (int k = 0;k < N; ++k)
       {
        
        ss << map[i][j][k].point_type;
       
       }
        ss << '\n'; 
     }

  }
cout << ss.str();
}

else //list output requested 
{

  ss << "//path taken" << "\n";
  while (!output_order.empty())
  {
    point_coord = output_order.back();
    output_order.pop_back(); 
    ss << "(" << point_coord.row << "," << point_coord.col << "," << point_coord.state << ",";
    ss << map[point_coord.state][point_coord.row][point_coord.col].point_type << ")" << "\n";
  }
cout << ss.str(); 
}




    return 0;

}


