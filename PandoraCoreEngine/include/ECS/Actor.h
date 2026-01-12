#pragma once
#include "Prerequisites.h"
#include "Entity.h"
#include "Buffer.h"
#include "Texture.h"
#include "Transform.h"
#include "SamplerState.h"
#include "ShaderProgram.h"

class Device;
class DeviceContext;
class MeshComponent;

/**
 * @class Actor
 * @brief Represents a graphical entity managing meshes, textures, and states.
 *
 * The Actor class encapsulates resources required to render an object in the
 * scene. It manages vertex/index buffers, rasterization states, blending
 * modes, and shader programs. It also handles specific rendering passes such
 * as shadow mapping.
 */
class
Actor : public Entity {
public:
  /**
   * @brief Default constructor.
   */
  Actor() = default;

  /**
   * @brief Constructs an Actor initialized with a graphics device.
   * @param device Reference to the Device for resource initialization.
   */
  Actor(Device& device);

  /**
   * @brief Virtual destructor.
   */
  virtual
  ~Actor() = default;

  void
  awake() override {}

  /**
   * @brief Initializes the actor.
   *
   * Inherited from Entity. Can be overridden for custom allocation.
   */
  void
  init() override {}

  /**
   * @brief Updates the actor's logic for the current frame.
   *
   * Handles time-dependent logic, animations, or transformations.
   *
   * @param deltaTime     Time elapsed since the last frame (seconds).
   * @param deviceContext Context for graphics operations during update.
   */
  void
  update(float deltaTime, DeviceContext& deviceContext) override;

  /**
   * @brief Renders the actor to the scene.
   *
   * Configures pipeline stages (states, buffers, shaders) and issues draw
   * calls for the associated meshes.
   *
   * @param deviceContext Context used for rendering operations.
   */
  void
  render(DeviceContext& deviceContext) override;

  /**
   * @brief Releases all resources associated with the actor.
   *
   * Frees vertex/index buffers, shaders, states, and texture references.
   */
  void
  destroy();

  /**
   * @brief Assigns mesh components and initializes hardware buffers.
   *
   * @param device The device used to create vertex/index buffers.
   * @param meshes Vector of MeshComponents for this actor.
   */
  void
  setMesh(Device& device, std::vector<MeshComponent> meshes);

  /**
   * @brief Retrieves the actor's name.
   * @return The string identifier of the actor.
   */
  std::string
  getName() { return m_name; }

  /**
   * @brief Sets the actor's name.
   * @param name The new name identifier.
   */
  void
  setName(const std::string& name) { m_name = name; }

  /**
   * @brief Assigns textures to the actor.
   * @param textures Vector of Texture objects to apply.
   */
  void
  setTextures(std::vector<Texture> textures) { m_textures = textures; }

  /**
   * @brief Enables or disables shadow casting.
   * @param v True to enable shadows, false to disable.
   */
  void
  setCastShadow(bool v) { castShadow = v; }

  /**
   * @brief Checks if the actor casts shadows.
   * @return True if shadow casting is enabled.
   */
  bool
  canCastShadow() const { return castShadow; }

  /**
   * @brief Performs the shadow render pass.
   *
   * Uses specific shadow shaders to render geometry into the shadow map.
   *
   * @param deviceContext Context used for shadow rendering.
   */
  void
  renderShadow(DeviceContext& deviceContext);

private:
  std::vector<MeshComponent> m_meshes;   ///< Actor's mesh components.
  std::vector<Texture> m_textures;       ///< Textures applied to meshes.
  std::vector<Buffer> m_vertexBuffers;   ///< Hardware vertex buffers.
  std::vector<Buffer> m_indexBuffers;    ///< Hardware index buffers.

  //BlendState m_blendstate;               ///< Blending state (disabled).
  //Rasterizer m_rasterizer;               ///< Rasterizer state (disabled).
  SamplerState m_sampler;                ///< Texture sampler state.
  CBChangesEveryFrame m_model;           ///< Per-frame constant buffer data.
  Buffer m_modelBuffer;                  ///< GPU constant buffer resource.

  // Shadow Mapping Resources
  ShaderProgram m_shaderShadow;          ///< Shader for shadow depth pass.
  Buffer m_shaderBuffer;                 ///< Auxiliary buffer for shadows.
  //BlendState m_shadowBlendState;         ///< Shadow blend state (disabled).
  //DepthStencilState m_sDSState;          ///< Shadow Depth/Stencil state.
  CBChangesEveryFrame m_cbShadow;        ///< Shadow constant buffer structure.

  XMFLOAT4 m_LightPos;                   ///< Light position for shadows.
  std::string m_name = "Actor";          ///< Unique actor identifier.
  bool castShadow = true;                ///< Flag for shadow casting.
};