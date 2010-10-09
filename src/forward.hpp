#ifndef FORWARD_HPP
#define FORWARD_HPP

#include "zb_tr1.hpp"

class Camera;
class ResourceManager;
class OpenGL;
class Level;
class Light;
class Model;
class Settings;
class RenderBuffer;
class Animator;
class Material;
struct InputEvent;
class Shader;
class Log;
class Entity;
class Scene;
class Player;
class InputHandler;
class Overlay;
class GLProgram;
class Game;
class Renderer;
class FrameBufferObject;
class FBOImage;
class State;
class Texture;
class RenderPass;
class SceneRenderer;
class PostProc;

typedef std::shared_ptr<FBOImage> FBOImagePtr;
typedef std::shared_ptr<FrameBufferObject> FBOPtr;
typedef std::shared_ptr<Texture> TexturePtr;

#endif // FORWARD_HPP
