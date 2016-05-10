#ifndef SPACEIMAGES_H
#define SPACEIMAGES_H

#include <string>
using namespace std;

class SpaceImage;

class SpaceImages
{
public:
    SpaceImages();
    ~SpaceImages();

    void push(SpaceImage* value);
    SpaceImage* pop();
    SpaceImage* getApod(std::string imageDate);

private:
    SpaceImage* first;
    SpaceImage* last;
    int nextId;

};

#endif // SPACEIMAGES_H

