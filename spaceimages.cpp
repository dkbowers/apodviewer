#include "spaceimages.h"
#include "SpaceImage.h"

SpaceImages::SpaceImages()
{
    first = NULL;
    last = NULL;
    nextId = 0;
}

SpaceImages::~SpaceImages()
{
}

void SpaceImages::push(SpaceImage* value)
{
    value->setId(nextId);
    nextId++;
    value->setPrevious(last);
    value->setNext(NULL);
    last = value;
}

SpaceImage* SpaceImages::pop()
{
    SpaceImage* myPopped = first;
    SpaceImage* myNext = first->getNext();
    first = myNext;
    //first->getPrevious(NULL);
    return 	myPopped;
}

SpaceImage* SpaceImages::getApod(std::string imageDate)
{
    // check if its in the image cache
    // if not in cache retrieve and add to cache
    return NULL;
}
