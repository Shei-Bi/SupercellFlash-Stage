#include "flash/Sprite.h"
#include "data/LogicData.h"
#include "flash/MovieClip.h"
#include "assert.h"
#include "ResourceManager.h"

class DataIcon :public Sprite {
public:
    LogicData* data;
    MovieClip* iconClip;
    DataIcon(LogicData* data) :Sprite() {
        iconClip = nullptr;
        data = nullptr;
        setData(data);
    }
    ~DataIcon() {
        if (iconClip) delete iconClip;
    }
    void setData(LogicData* data) {
        if (iconClip) {
            delete iconClip;
        }
        iconClip = nullptr;
        this->data = data;
        if (data) {
            assert(data->getIconSWF().length() > 0 && data->getIconExportName().length() > 0);
            setIconClip(data->getIconSWF(), data->getIconExportName());
        }
    }
    void setIconClip(const std::string& file, const std::string& name) {
        iconClip = ResourceManager::getMovieClip(file, name);
        iconClip->gotoAndPlayFrameIndex(0, -1);
        addChild(iconClip);
    }
    void replaceInstanceWithIcon(MovieClip* target, const char* name, int scalingType, int idk) {
        auto child = target->getChildByName(name);
        if (child) {
            int childIndex = target->getChildIndex(child);
            setPixelSnappedXY(child->getX(), child->getY());
            iconClip->setPixelSnappedXY(0.0f, 0.0f);
            target->addChildAt(this, childIndex);
        }
    }
};