#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {
      mapImg = tm;
      start = s;

      vector<vector<bool>> visited(mapImg.width(), vector<bool>(mapImg.height()));
      vector<vector<int>> length(mapImg.width() ,vector<int>(mapImg.height()));
      vector<vector<pair<int, int>>> pastPts(mapImg.width(), vector<pair<int, int>>(mapImg.height()));

      Queue<pair<int,int>> location;
      visited[start.first][start.second] = true;
      length[start.first][start.second] = 0;
      location.enqueue(start);
      
      // pastPts[start.first][start.second] = s;
      pair<int,int> curr = start;
      while(!location.isEmpty()){
         curr = location.dequeue();
         vector<pair<int,int>> neighbersVector = neighbors(curr);

         for(int i = 0; i < (int)neighbersVector.size(); i++){
            pair<int,int> p = neighbersVector[i];
            if(good(visited, length, curr, p)){
               length[p.first][p.second] = length[curr.first][curr.second]+1;
               visited[p.first][p.second] = true;
               pastPts[p.first][p.second] = curr;
               
               location.enqueue(p);
            }
         }
      }
      pathPts.push_back(start);
      mazePts.push_back(curr);
      while(pastPts[curr.first][curr.second] != start){
         mazePts.push_back(pastPts[curr.first][curr.second]);
         curr = pastPts[curr.first][curr.second];
         pathPts.push_back(pastPts[curr.first][curr.second]);
      }
      mazePts.push_back(start);
   }

PNG decoder::renderSolution(){

   PNG baseCopy = mapImg;

   for (int i = (int) mazePts.size() - 1 ; i >= 0; i-- ) {
      pair<int, int> mazeLoc = mazePts[i];
      RGBAPixel *curr = baseCopy.getPixel(mazeLoc.first, mazeLoc.second);
      curr->r = 255;
      curr->g = 0;
      curr->b = 0;

   }
   return baseCopy;
}


 // discovers and draws the entire maze on top of the given image. 
    // The maze is revealed by darkening each of the maze pixels: change 
    // each colour channel to be 50% of its treasure map value. (use the
    // setGrey function from the treasureMap class.)
    //
    // Discovering the maze requires yet another breadth-first traversal
    // of the treasureMap image. (you may be able to facilitate this
    // within the constructor by adding another member variable to the 
    // decoder class. We leave that design decision up to you.)
    //
    // Note that the grey pixels in the maze rendered by this function 
    // may be a superset of the pixels in the original maze, because 
    // some neighboring pixels might have the "correct" lower
    // order bits, just by happenstance.
    //
    // This function also draws a red 7px x 7px square at the start 
    // location. If the start location is near the edge of the image,
    // just draw the existing portion of the square. The start 
    // should be at the center of the square.
PNG decoder::renderMaze(){

   PNG base = mapImg;
    
      vector<vector<bool>> visited(base.width(), vector<bool>(base.height()));
      vector<vector<int>> length(base.width() ,vector<int>(base.height()));
      vector<vector<pair<int, int>>> pastPts(base.width(), vector<pair<int, int>>(base.height()));

      Queue<pair<int,int>> location;
      visited[start.first][start.second] = true;
      length[start.first][start.second] = 0;
      location.enqueue(start);
      setGrey(base, start);
      // pastPts[start.first][start.second] = s;
      pair<int,int> curr = start;
      while(!location.isEmpty()){
         curr = location.dequeue();
         vector<pair<int,int>> neighbersVector = neighbors(curr);
         for(int i = 0; i < (int)neighbersVector.size(); i++){
            pair<int,int> p = neighbersVector[i];
            if(good(visited, length, curr, p)){
               length[p.first][p.second] = length[curr.first][curr.second]+1;
               visited[p.first][p.second] = true;
               pastPts[p.first][p.second] = curr;
               setGrey(base, p);
               location.enqueue(p);
            }
         }
      }


        for (int i = -3; i < 4; i++) {
        for (int j = -3; j < 4; j++) {
            if ((start.first + i >= 0) && (start.second + j >= 0) && (start.first < (int)mapImg.width()) && (start.second < (int)mapImg.height()) ) {
              RGBAPixel *curr = base.getPixel(start.first + i, start.second + j);
              if(curr) {
                  curr->r = 255;
                  curr->g = 0;
                  curr->b = 0;
              }
            }

        }
    }

    return base;
}

void decoder::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
  RGBAPixel* pixel = im.getPixel(loc.first, loc.second);
  pixel->r = 2*(pixel->r/4);
  pixel->g = 2*(pixel->g/4);
  pixel->b = 2*(pixel->b/4);
}

pair<int,int> decoder::findSpot(){

/* YOUR CODE HERE */
return pathPts.back();
}

int decoder::pathLength(){

/* YOUR CODE HERE */
return pathPts.size()+1;
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
bool ans = false;
/* YOUR CODE HERE */
  if(0<=next.first&& next.first<(int)mapImg.width()){
    if(0<=next.second&& next.second<(int)mapImg.height()){
      if(v[next.first][next.second] == false){
         RGBAPixel *pixel = mapImg.getPixel(next.first,next.second);
        if(compare(*pixel, (d[curr.first][curr.second]))){
          ans = true;
        }
      }
    }
  }else{
    ans =  false;
  }  
  return ans;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */
vector<pair<int, int>> retval;
    retval.push_back(make_pair(curr.first-1, curr.second));
    retval.push_back(make_pair(curr.first, curr.second+1));
    retval.push_back(make_pair(curr.first+1, curr.second));
    retval.push_back(make_pair(curr.first, curr.second-1));
    return retval;
}


bool decoder::compare(RGBAPixel p, int d){

/* YOUR CODE HERE */
bool ans = false;
if(((p.r%4)*16 + (p.g%4)*4 + p.b%4) == (d+1)%64){
   ans = true;
}else{
   ans = false;
}
return ans;
}
