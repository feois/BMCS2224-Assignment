
#ifndef SCENE_HPP
#define SCENE_HPP

#include "utils.hpp"

template<typename C>
class Engine;

// a scene that receives a context
template<typename C>
class Scene {
protected:
    Engine<C> *engine = nullptr; // the engine pointer
    C* context = nullptr; // the context parameter
    Box<UI> ui; // the UI engine will render
    
    friend Engine<C>;
    
public:
    // called when engine and context is set
    virtual void init() {}
    // called in Engine::physics
    virtual void physics() {}
    // called in Engine::render
    virtual void render() {}
    
    virtual ~Scene() = default;
};

#endif
