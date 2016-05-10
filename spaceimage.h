#ifndef SPACEIMAGE_H
#define SPACEIMAGE_H
#include <string>

class SpaceImage
{
public:
    SpaceImage();
    ~SpaceImage();

    int getId() { return id; }
    void setId(int value) { id = value; }
    SpaceImage* getPrevious() { return previous; }
    void setPrevious(SpaceImage* value) { previous = value; }
    SpaceImage* getNext() { return next; }
    void setNext(SpaceImage* value) { next = value; }

    std::string getCopyright() { return copyright; }
    void setCopyright(std::string value) { copyright = value; }
    std::string getDate() { return date; }
    void setDate(std::string value) { date = value; }
    std::string getExplanation() { return explanation; }
    void setExplanation(std::string value) { explanation = value; }
    std::string getHDUrl() { return hdurl; }
    void setHDUrl(std::string value) { hdurl = value; }
    std::string getMediaType() { return media_type; }
    void setMediaType(std::string value) { media_type = value; }
    std::string getServiceVersion() { return service_version; }
    void setServiceVersion(std::string value) { service_version = value; }
    std::string getTitle() { return title; }
    void setTitle(std::string value) { title = value; }
    std::string getUrl() { return url; }
    void setUrl(std::string value) { url = value; }

private:
    int id;
    SpaceImage* previous;
    SpaceImage* next;

    std::string copyright;			// "Yuri Beletsky"
    std::string date;				// "2015 - 10 - 01"
    std::string explanation;		// "This stunning panorama in southern skies was recorded on the colorful night of September 27 / 28 from Carnegie Las Campanas Observatory.A diffuse glow and dark rifts of the central Milky Way hang over domes of the twin 6.5 meter Magellan telescopes.But most eye - catching is the deep red glow of the Moon.Immersed in Earth's shadow during the much anticipated perigee-total-lunar eclipse, the Moon's surface reflects the light of sunsets and sunrises scattered and refracted into the planet's cone-shaped umbra. Along with the dramatic hue of the eclipsed Moon, other colors of that night captured by the sensitive digital camera include the red and green shades of atmospheric airglow. Viewers can also spot the Andromeda Galaxy below the Moon, seen as a tiny smudge through the reddish airglow and lights along the horizon. The Magellanic Clouds, satellite galaxies of the Milky Way, join in at the far left of the full panorama frame.",
    std::string hdurl;				// "http://apod.nasa.gov/apod/image/1510/lunareclipse_27Sep_beletsky.jpg",
    std::string media_type;			// "image",
    std::string service_version;	// "v1",
    std::string title;				// "Eclipsed in Southern Skies",
    std::string url;				// "http://apod.nasa.gov/apod/image/1510/lunareclipse_27Sep_beletskycrop4.jpg"

};


#endif // SPACEIMAGE_H
