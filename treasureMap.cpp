#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

/* YOUR CODE HERE */
    start = s;
    base = baseim;
    maze = mazeim;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
RGBAPixel* pixel = im.getPixel(loc.first, loc.second);
  pixel->r = 2*(pixel->r/4);
  pixel->g = 2*(pixel->g/4);
  pixel->b = 2*(pixel->b/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

/* YOUR CODE HERE */
RGBAPixel* pixel = im.getPixel(loc.first, loc.second);
int d6 = d%64;
pixel->r = (pixel->r & ~ 3)|d6/16%4;
pixel->g = (pixel->g & ~ 3)|d6/4%4;
pixel->b = (pixel->b & ~ 3)|d6%4;
}


// draws the entire maze on a copy of the base image inside the low order bits.
    // in this encoding, each pixel differs by very little from its 
    // original value. The returned PNG will be visually identical to the base,
    // but anyone with a decoder can find and solve the maze. (We specify the
    // computation of the encoded colours in the description of a setLOB function,
    // below.)
    
    // This function does a "breadth-first" traversal of the maze image, and at each
    // iteration of the traversal, it encodes the current location as a part of the
    // maze in a copy of the base image. The algorithm is spelled out carefully
    // for this function. You will want to adapt it to design solutions
    // to some of the other functions in this assignment.
    
    // 0. Make a copy of the base image. You will change its pixels corresponding to 
    //      locations within the maze.
    // 1. Initialize working vectors: 
    //      a. Define a 2d vector whose dimensions are the same as the image, and whose 
    //      values are boolean. Use this vector to indicate whether or not a location
    //      has been visited by the traversal.
    //      b. Define a 2d vector whose dimensions are the same as the image, and whose
    //      values are integers. This structure will be used to store the length of the
    //      shortest path between the current location and the start location.
    // 2. Initialize a queue whose purpose is to orchestrate the traversal. As you search
    //      the maze, you will have to keep a list of locations to explore. The queue maintains
    //      that list.
    // 3. For the start location, do the following:
    //      a. mark the start location as visited (structure in step 1a)
    //      b. set the distance to the start to be 0 (structure in step 1b)
    //      c. encode the distance within the start location's lower order bits of the treasure map 
    //          (structure in step 0)
    //      d. enqueue the start location. (structure in step 2)
    // 4. While the queue is not empty:
    //      a. set the current location, curr, to the value at the front of the queue (and dequeue). 
    //      b. for each compass neighbor, p, of curr in the order of left, below, right, above:
    //          i. if p is "good" (defined below) then
    //              1) mark p's location as visited
    //              2) set p's distance to the start to be curr's distance, + 1. 
    //              3) encode the distance in the lower order bits of the treasure map
    //              4) enqueue p
         
    // 5. return the image
PNG treasureMap::renderMap(){

/* YOUR CODE HERE */
    PNG img = base;
    vector<vector<bool>> visited(base.width(), vector<bool>(base.height()));
    vector<vector<int>> length(base.width() ,vector<int>(base.height()));
    Queue<pair<int,int>> location;
    visited[start.first][start.second] = true;
    length[start.first][start.second] = 0;
    location.enqueue(start);
    setLOB(img, start, length[start.first][start.second]);
    while(!location.isEmpty()){
      pair<int,int> curr = location.dequeue();
      vector<pair<int,int>> neighbersVector = neighbors(curr);
      for(int i = 0; i < (int)neighbersVector.size(); i++){
        pair<int,int> p = neighbersVector[i];
        if(good(visited, curr, p)){
          length[p.first][p.second] = length[curr.first][curr.second]+1;
          visited[p.first][p.second] = (bool) true;

          setLOB(img, p, length[p.first][p.second]);
          location.enqueue(p);
        }
      }
    }
    return img;
  }


	//draws the entire maze on a copy of the base image by darkening the locations
    // so it's visible as a shadow. This can be done by iterating over every
    // pixel, but it will be faster to begin from the start position
    // and find the points in the maze from there. Returns an image whose 
    // non-maze pixels are the same as the base image, and whose maze pixels 
    // have colour values that are 50% of those in the base image, for each of 
    // red, green and blue. (We specify the computation of the colours in the 
    // description of a setGrey function, below.) 
    //
    // This function also draws a red 7px x 7px square at the start                                                                                    
    // location. If the start location is near the edge of the image,
    // just draw the existing portion of the square. The start 
    // should be at the center of the square.
PNG treasureMap::renderMaze(){
   PNG baseCopy = base;


    vector<vector<bool>> visited(base.width(), vector<bool>(base.height()));
    vector<vector<int>> length(base.width() ,vector<int>(base.height()));
    Queue<pair<int,int>> location;
    visited[start.first][start.second] = true;
    length[start.first][start.second] = 0;
    location.enqueue(start);
    setGrey(baseCopy, start);
    while(!location.isEmpty()){
      pair<int,int> curr = location.dequeue();
      vector<pair<int,int>> neighbersVector = neighbors(curr);
      for(int i = 0; i < (int)neighbersVector.size(); i++){
        pair<int,int> p = neighbersVector[i];
        if(good(visited, curr, p)){
          length[p.first][p.second] = length[curr.first][curr.second]+1;
          visited[p.first][p.second] = (bool) true;

          setGrey(baseCopy, p);
          location.enqueue(p);
        }
      }
    }


        for (int i = -3; i < 4; i++) {
        for (int j = -3; j < 4; j++) {
            if ((start.first + i >= 0) && (start.second + j >= 0) && (start.first < (int)base.width()) && (start.second < (int)base.height()) ) {
              RGBAPixel *curr = baseCopy.getPixel(start.first + i, start.second + j);
              if(curr) {
                  curr->r = 255;
                  curr->g = 0;
                  curr->b = 0;
              }
            }

        }
    }

    return baseCopy;


}
// tests next, a neighbor (adjacent vertex) of curr to see if it is 
    // 1. within the image, 2. unvisited, and 3. the same colour as curr in
    // the maze image (member var).  An entry in table v is true if a cell 
    // has previously been visited, and false, otherwise. Colour sameness is
    // measured using the == operator on RGBAPixel values.
bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
bool ans = false;
/* YOUR CODE HERE */
  if(0<=next.first&& next.first<(int)base.width()){
    if(0<=next.second&& next.second<(int)base.height()){
      if(v[next.first][next.second] == false){
        if(*maze.getPixel(curr.first,curr.second) == *maze.getPixel(next.first, next.second)){
          ans = true;
        }
      }
    }
  }else{
    ans =  false;
  }
  return ans;

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

    // TODO: CHECK
    vector<pair<int, int>> retval;
    retval.push_back(make_pair(curr.first-1, curr.second));
    retval.push_back(make_pair(curr.first, curr.second-1));
    retval.push_back(make_pair(curr.first+1, curr.second));
    retval.push_back(make_pair(curr.first, curr.second+1));
    return retval;
    

}

