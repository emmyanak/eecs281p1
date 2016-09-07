//
//  location.h
//  281project1
//
//  Created by Emma Yanakiev on 5/5/16.
//  
//

#ifndef ___81project1__location__
#define ___81project1__location__

#include <stdio.h>
    
    typedef struct {
    
    bool visited; 
    char point_type, direction;
    
  } Location;

   typedef struct {

   int state, row, col;
  } Path; 

#endif /* defined(___81project1__location__) */
