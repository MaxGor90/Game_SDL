#ifndef LAYER_H
#define LAYER_H


class Layer
{
public:

    enum Type
    {
        TILE,
        IMAGE
    };

    Type LayerType;
    
    virtual void Render() = 0;
    virtual void Update() = 0;
}; 

#endif /* LAYER_H */
