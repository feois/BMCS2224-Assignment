
#ifndef SCENE_HPP
#define SCENE_HPP

#include "utils.hpp"

template<typename C>
class Engine;

template<typename C>
class Scene {
protected:
    Engine<C> *engine = nullptr;
    C* context = nullptr;
    Box<UI> ui;
    
    friend Engine<C>;
    
public:
    virtual void init() {}
    virtual void physics() {}
    virtual void render() {}
    
    virtual ~Scene() = default;
};

#endif
