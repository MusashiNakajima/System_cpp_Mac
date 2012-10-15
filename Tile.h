//
//  Tile.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/14.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Tile_h
#define _Tile_h

#define TH  5

class Tile
{
public:
    int		tID;
    int     mColor;
    char    XBeeAddr64[8];
	char	XBeeAddr16[2];
    
    Tile(int t, char *a64, char *a16);
    ~Tile();
   
	void    isAlive();
    int     deadCheck();
	
private:
	int     dead, alive, standby;
    int     deadCount;

};

#endif
