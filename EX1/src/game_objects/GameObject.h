#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
protected:
    bool is_exploded;

public:
    GameObject(/* args */): is_exploded(false) {}
    virtual ~GameObject();
    bool getIsExploded() const;
    void setIsExploded(bool exploded);
};

#endif