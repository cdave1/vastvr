#include "BuilderSerialization.h"
#include "json/json.h"
#include <cstdio>

const std::string NAME_KEY = "Name";
const std::string COMPONENTS_KEY = "components";
const std::string COMPONENT_TYPE_KEY = "component_type";
const std::string DATA_KEY = "data";

const std::string ORIGIN_KEY = "origin";
const std::string SCALE_KEY = "scale";
const std::string ROTATION_KEY = "rotation";

const std::string MESH_METHOD_KEY = "meshMethod";
const std::string CYLINDER_RADIUS_KEY = "cylinderRadius";
const std::string CYLINDER_LENGTH_KEY = "cylinderLength";
const std::string CYLINDER_SEGMENTS_KEY = "cylinderSegments";
const std::string QUAD_AABB_KEY = "quadAABB";
const std::string CUBE_AABB_KEY = "cubeAABB";
const std::string SPHERE_RADIUS_KEY = "sphereRadius";
const std::string SPHERE_SEGMENTS_KEY = "sphereSegments";

const std::string SIMPLE_CUBE_SIZE_KEY = "simpleCubeSize";
const std::string REPEATING_GRID_AABB_KEY = "repeatingGridAABB";
const std::string REPEATING_GRID_CUBE_SIZE_KEY = "repeatingGridCubeSize";
const std::string REPEATING_GRID_GAP_SIZE_KEY = "repeatingGridGapSize";

const std::string BOX_WITH_CURVED_CORNERS_SIZE_KEY = "boxWithCurvedCornersSize";
const std::string BOX_WITH_CURVED_CORNERS_RADIUS_KEY = "boxWithCurvedCornersCornerRadius";
const std::string MESH_PATH_KEY = "meshFilePath";

const std::string GAP_GRID_SIZE = "gapGridSize";
const std::string GAP_GRID_NODE_SIZE = "gapGridNodeSize";

const std::string MATERIAL_TEXTURE_KEY = "texture";
const std::string MATERIAL_NORMAL_TEXTURE_KEY = "normalTexture";
const std::string MATERIAL_ALPHA_TEXTURE_KEY = "alphaMaskTexture";
const std::string MATERIAL_BUMP_TEXTURE_KEY = "bumpTexture";
const std::string MATERIAL_AMBIENT_COLOR_KEY = "ambientColor";
const std::string MATERIAL_DIFFUSE_COLOR_KEY = "diffuseColor";
const std::string MATERIAL_SPECULAR_COLOR_KEY = "specularColor";
const std::string MATERIAL_EMISSION_COLOR_KEY = "emissionColor";
const std::string MATERIAL_AMBIENT_ADJUSTMENT_KEY = "ambientAdjustmentColor";
const std::string MATERIAL_SHININESS_KEY = "shininessKey";

const std::string HEADSET_WORLD_POSITION = "headsetWorldPosition";



using namespace hd;

bool BuilderSerialization::SaveToJSON(hd::EntityPool *entityPool, const std::string &name) {
    // For each entity, we actually only need to save the data that is independant for that
    // entity.  We need only save enough information to write the entity to disk and restore it.
    //
    // It's possible that this entails saving a history of changes to each entity, and then replaying
    // those changes?

    EntityQuery *blocks = entityPool->Query<graphics::Shader, graphics::MeshReference,
        graphics::Material, graphics::Transform, hd::input::LookAtResponder>(name);

    Json::Value value;
    Json::Value entitiesJson(Json::arrayValue);

    for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);

        auto shader = entity->Get<graphics::Shader>();
        auto mesh = entity->Get<graphics::MeshReference>();
        auto transform = entity->Get<graphics::Transform>();
        auto material = entity->Get<graphics::Material>();

        Json::Value entityJson;
        Json::Value componentsJson;
        entityJson[NAME_KEY] = entity->GetName();

#ifdef WIN32
        const auto id = entity->GetUUID();
        char uuidText[37];
        std::snprintf(uuidText, sizeof(uuidText),
                      "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                      static_cast<unsigned long>(id.Data1),
                      static_cast<unsigned int>(id.Data2), static_cast<unsigned int>(id.Data3),
                      static_cast<unsigned int>(id.Data4[0]), static_cast<unsigned int>(id.Data4[1]),
                      static_cast<unsigned int>(id.Data4[2]), static_cast<unsigned int>(id.Data4[3]),
                      static_cast<unsigned int>(id.Data4[4]), static_cast<unsigned int>(id.Data4[5]),
                      static_cast<unsigned int>(id.Data4[6]), static_cast<unsigned int>(id.Data4[7]));
        entityJson["UUID"] = uuidText;
#else
        char uuidStr[40];
        uuid_unparse(* entity->GetUUID(), uuidStr);
        entityJson["UUID"] = std::string(uuidStr);
#endif

        if (transform) {
            Vec3 origin = transform->GetOrigin();
            Vec3 scale = transform->GetScale();
            Quaternion rotation = transform->GetRotation();

            Json::Value transformJson;
            transformJson[COMPONENT_TYPE_KEY] = "transform";
            transformJson[DATA_KEY][ORIGIN_KEY].append(Json::Value(origin[0]));
            transformJson[DATA_KEY][ORIGIN_KEY].append(Json::Value(origin[1]));
            transformJson[DATA_KEY][ORIGIN_KEY].append(Json::Value(origin[2]));

            transformJson[DATA_KEY][SCALE_KEY].append(Json::Value(scale[0]));
            transformJson[DATA_KEY][SCALE_KEY].append(Json::Value(scale[1]));
            transformJson[DATA_KEY][SCALE_KEY].append(Json::Value(scale[2]));

            transformJson[DATA_KEY][ROTATION_KEY].append(Json::Value(rotation[0]));
            transformJson[DATA_KEY][ROTATION_KEY].append(Json::Value(rotation[1]));
            transformJson[DATA_KEY][ROTATION_KEY].append(Json::Value(rotation[2]));
            transformJson[DATA_KEY][ROTATION_KEY].append(Json::Value(rotation[3]));

            componentsJson.append(transformJson);
        }

        if (mesh) {
            hd::graphics::MeshPrototype prototype = mesh->GetMeshData()->GetPrototype();

            Json::Value meshJson;
            meshJson[COMPONENT_TYPE_KEY] = "mesh";
            meshJson[DATA_KEY][MESH_METHOD_KEY] = Json::Value(int(prototype.meshMethod));
            meshJson[DATA_KEY][CYLINDER_RADIUS_KEY] = Json::Value(prototype.cylinderRadius);
            meshJson[DATA_KEY][CYLINDER_LENGTH_KEY] = Json::Value(prototype.cylinderLength);
            meshJson[DATA_KEY][CYLINDER_SEGMENTS_KEY] = Json::Value(prototype.cylinderSegments);

            meshJson[DATA_KEY][QUAD_AABB_KEY].append(Json::Value(prototype.quadAABB.lower[0]));
            meshJson[DATA_KEY][QUAD_AABB_KEY].append(Json::Value(prototype.quadAABB.lower[1]));
            meshJson[DATA_KEY][QUAD_AABB_KEY].append(Json::Value(prototype.quadAABB.lower[2]));
            meshJson[DATA_KEY][QUAD_AABB_KEY].append(Json::Value(prototype.quadAABB.upper[0]));
            meshJson[DATA_KEY][QUAD_AABB_KEY].append(Json::Value(prototype.quadAABB.upper[1]));
            meshJson[DATA_KEY][QUAD_AABB_KEY].append(Json::Value(prototype.quadAABB.upper[2]));

            meshJson[DATA_KEY][CUBE_AABB_KEY].append(Json::Value(prototype.cubeAABB.lower[0]));
            meshJson[DATA_KEY][CUBE_AABB_KEY].append(Json::Value(prototype.cubeAABB.lower[1]));
            meshJson[DATA_KEY][CUBE_AABB_KEY].append(Json::Value(prototype.cubeAABB.lower[2]));
            meshJson[DATA_KEY][CUBE_AABB_KEY].append(Json::Value(prototype.cubeAABB.upper[0]));
            meshJson[DATA_KEY][CUBE_AABB_KEY].append(Json::Value(prototype.cubeAABB.upper[1]));
            meshJson[DATA_KEY][CUBE_AABB_KEY].append(Json::Value(prototype.cubeAABB.upper[2]));

            meshJson[DATA_KEY][SPHERE_RADIUS_KEY] = Json::Value(prototype.sphereRadius);
            meshJson[DATA_KEY][SPHERE_SEGMENTS_KEY] = Json::Value(prototype.sphereSegments);

            meshJson[DATA_KEY][SIMPLE_CUBE_SIZE_KEY].append(Json::Value(prototype.simpleCubeSize[0]));
            meshJson[DATA_KEY][SIMPLE_CUBE_SIZE_KEY].append(Json::Value(prototype.simpleCubeSize[1]));
            meshJson[DATA_KEY][SIMPLE_CUBE_SIZE_KEY].append(Json::Value(prototype.simpleCubeSize[2]));

            meshJson[DATA_KEY][REPEATING_GRID_AABB_KEY].append(Json::Value(prototype.repeatingGridAABB.lower[0]));
            meshJson[DATA_KEY][REPEATING_GRID_AABB_KEY].append(Json::Value(prototype.repeatingGridAABB.lower[1]));
            meshJson[DATA_KEY][REPEATING_GRID_AABB_KEY].append(Json::Value(prototype.repeatingGridAABB.lower[2]));
            meshJson[DATA_KEY][REPEATING_GRID_AABB_KEY].append(Json::Value(prototype.repeatingGridAABB.upper[0]));
            meshJson[DATA_KEY][REPEATING_GRID_AABB_KEY].append(Json::Value(prototype.repeatingGridAABB.upper[1]));
            meshJson[DATA_KEY][REPEATING_GRID_AABB_KEY].append(Json::Value(prototype.repeatingGridAABB.upper[2]));

            meshJson[DATA_KEY][REPEATING_GRID_CUBE_SIZE_KEY].append(Json::Value(prototype.repeatingGridCubeSize[0]));
            meshJson[DATA_KEY][REPEATING_GRID_CUBE_SIZE_KEY].append(Json::Value(prototype.repeatingGridCubeSize[1]));
            meshJson[DATA_KEY][REPEATING_GRID_CUBE_SIZE_KEY].append(Json::Value(prototype.repeatingGridCubeSize[2]));

            meshJson[DATA_KEY][REPEATING_GRID_GAP_SIZE_KEY].append(Json::Value(prototype.repeatingGridGapSize[0]));
            meshJson[DATA_KEY][REPEATING_GRID_GAP_SIZE_KEY].append(Json::Value(prototype.repeatingGridGapSize[1]));
            meshJson[DATA_KEY][REPEATING_GRID_GAP_SIZE_KEY].append(Json::Value(prototype.repeatingGridGapSize[2]));

            meshJson[DATA_KEY][BOX_WITH_CURVED_CORNERS_SIZE_KEY].append(Json::Value(prototype.boxWithCurvedCornersSize[0]));
            meshJson[DATA_KEY][BOX_WITH_CURVED_CORNERS_SIZE_KEY].append(Json::Value(prototype.boxWithCurvedCornersSize[1]));
            meshJson[DATA_KEY][BOX_WITH_CURVED_CORNERS_SIZE_KEY].append(Json::Value(prototype.boxWithCurvedCornersSize[2]));
            meshJson[DATA_KEY][BOX_WITH_CURVED_CORNERS_RADIUS_KEY] = Json::Value(prototype.boxWithCurvedCornersCornerRadius);

            meshJson[DATA_KEY][MESH_PATH_KEY] = Json::Value(prototype.meshFilePath);

            componentsJson.append(meshJson);
        }

        if (shader) {
            Json::Value shaderJson;
            //shaderJson[DATA_KEY][SHADER_FRAGMENT_SHADER] = shader->
        }

        if (material) {
            Json::Value materialJson;
            materialJson[COMPONENT_TYPE_KEY] = "material";
            if (material->GetTexture()) {
                materialJson[DATA_KEY][MATERIAL_TEXTURE_KEY] = std::string(material->GetTexture()->name);
            }

            if (material->GetNormalTexture()) {
                materialJson[DATA_KEY][MATERIAL_NORMAL_TEXTURE_KEY] = std::string(material->GetNormalTexture()->name);
            }

            if (material->GetAlphaMaskTexture()) {
                materialJson[DATA_KEY][MATERIAL_ALPHA_TEXTURE_KEY] = std::string(material->GetAlphaMaskTexture()->name);
            }

            if (material->GetBumpTexture()) {
                materialJson[DATA_KEY][MATERIAL_BUMP_TEXTURE_KEY] = std::string(material->GetBumpTexture()->name);
            }

            materialJson[DATA_KEY][MATERIAL_AMBIENT_COLOR_KEY].append(Json::Value(material->GetAmbient()[0]));
            materialJson[DATA_KEY][MATERIAL_AMBIENT_COLOR_KEY].append(Json::Value(material->GetAmbient()[1]));
            materialJson[DATA_KEY][MATERIAL_AMBIENT_COLOR_KEY].append(Json::Value(material->GetAmbient()[2]));
            materialJson[DATA_KEY][MATERIAL_AMBIENT_COLOR_KEY].append(Json::Value(material->GetAmbient()[3]));

            materialJson[DATA_KEY][MATERIAL_DIFFUSE_COLOR_KEY].append(Json::Value(material->GetDiffuse()[0]));
            materialJson[DATA_KEY][MATERIAL_DIFFUSE_COLOR_KEY].append(Json::Value(material->GetDiffuse()[1]));
            materialJson[DATA_KEY][MATERIAL_DIFFUSE_COLOR_KEY].append(Json::Value(material->GetDiffuse()[2]));
            materialJson[DATA_KEY][MATERIAL_DIFFUSE_COLOR_KEY].append(Json::Value(material->GetDiffuse()[3]));

            materialJson[DATA_KEY][MATERIAL_SPECULAR_COLOR_KEY].append(Json::Value(material->GetSpecular()[0]));
            materialJson[DATA_KEY][MATERIAL_SPECULAR_COLOR_KEY].append(Json::Value(material->GetSpecular()[1]));
            materialJson[DATA_KEY][MATERIAL_SPECULAR_COLOR_KEY].append(Json::Value(material->GetSpecular()[2]));
            materialJson[DATA_KEY][MATERIAL_SPECULAR_COLOR_KEY].append(Json::Value(material->GetSpecular()[3]));

            materialJson[DATA_KEY][MATERIAL_EMISSION_COLOR_KEY].append(Json::Value(material->GetEmission()[0]));
            materialJson[DATA_KEY][MATERIAL_EMISSION_COLOR_KEY].append(Json::Value(material->GetEmission()[1]));
            materialJson[DATA_KEY][MATERIAL_EMISSION_COLOR_KEY].append(Json::Value(material->GetEmission()[2]));
            materialJson[DATA_KEY][MATERIAL_EMISSION_COLOR_KEY].append(Json::Value(material->GetEmission()[3]));

            materialJson[DATA_KEY][MATERIAL_AMBIENT_ADJUSTMENT_KEY].append(Json::Value(material->GetAmbientAdjustment()[0]));
            materialJson[DATA_KEY][MATERIAL_AMBIENT_ADJUSTMENT_KEY].append(Json::Value(material->GetAmbientAdjustment()[1]));
            materialJson[DATA_KEY][MATERIAL_AMBIENT_ADJUSTMENT_KEY].append(Json::Value(material->GetAmbientAdjustment()[2]));
            materialJson[DATA_KEY][MATERIAL_AMBIENT_ADJUSTMENT_KEY].append(Json::Value(material->GetAmbientAdjustment()[3]));

            materialJson[DATA_KEY][MATERIAL_SHININESS_KEY] = Json::Value(material->GetShininess());

            componentsJson.append(materialJson);
        }

        entityJson[COMPONENTS_KEY] = componentsJson;

        entitiesJson.append(entityJson);
    }

    value["entities"] = entitiesJson;

    Json::FastWriter writer;
    std::string json = writer.write(value);

    const std::string myFilePath = "saves/MySaveFile.json";
    if (hd::FileSystem::WriteToFile(myFilePath, json.data(), json.size()) != 0) {
        hd::log::e("[Builder]: Could not save %s\n", myFilePath.c_str());
        return false;
    }
    return true;
}


void BuilderSerialization::LoadFromJSON(hd::EntityPool *entityPool, std::function<void(hd::Entity *entity)> onRestoreCallback) {
    std::string path = "saves/MySaveFile.json";
    hd::File *bufferFile = NULL;
    if (NULL == (bufferFile = hd::FileSystem::OpenFile(path.c_str()))) {
        hd::log::i("[Builder]: Could not open (%s) for reading. Couldn't find file\n", path.c_str());
        return;
    }

    char *fileBuffer = NULL;
    int fileLength = hd::FileSystem::GetFileSize(bufferFile);
    if (fileLength == 0 || NULL == (fileBuffer = (char *)calloc(1, size_t(fileLength) + 1))) {
        hd::log::e("CALLOC!\n");
        hd::FileSystem::CloseFile(bufferFile);
        return;
    }

    hd::FileSystem::ReadFile(fileBuffer, fileLength, 1, bufferFile);
    hd::FileSystem::CloseFile(bufferFile);

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(fileBuffer, fileBuffer + fileLength, root)) {
        hd::log::e("Error reading json!\n");
        hdFreeSafely(fileBuffer);
        return;
    }

    hdFreeSafely(fileBuffer);

    Json::Value entitiesJson = root["entities"];
    if (entitiesJson.isArray()) {
        Json::ArrayIndex entityIndex = 0;
        Json::ArrayIndex entitySize = entitiesJson.size();

        for (entityIndex = 0; entityIndex < entitySize; ++entityIndex) {
            Json::Value entityJson = entitiesJson[entityIndex];

            Entity *newItem = entityPool->Create();
            if (entityJson.isMember(NAME_KEY)) {
                newItem->SetName(entityJson[NAME_KEY].asString());
            }

            auto transform = newItem->Add<hd::graphics::Transform>();
            auto shader = newItem->Add<hd::graphics::Shader>();
            auto mesh = newItem->Add<hd::graphics::MeshReference>();
            auto material = newItem->Add<hd::graphics::Material>();
            auto identifier = newItem->Add<ui::Identifier>();
            auto collisionData = newItem->Add<hd::graphics::CollisionData>();
            auto opacityData = newItem->Add<hd::graphics::OpacityData>();

            collisionData->OnGetTransform([transform]() {
                return transform->GetMatrix();
            });

            if (entityJson.isMember(COMPONENTS_KEY) &&
                entityJson[COMPONENTS_KEY].isArray()) {

                Json::Value componentsJson = entityJson[COMPONENTS_KEY];

                Json::ArrayIndex componentIndex = 0;
                Json::ArrayIndex componentSize = componentsJson.size();

                for (componentIndex = 0; componentIndex < componentSize; ++componentIndex) {
                    Json::Value componentJson = componentsJson[componentIndex];

                    if (componentJson.isMember(COMPONENT_TYPE_KEY)) {
                        std::string componentType = componentJson[COMPONENT_TYPE_KEY].asString();
                        Json::Value componentData = componentJson[DATA_KEY];

                        if (componentType == "transform") {
                            Vec3 origin(componentData[ORIGIN_KEY][0].asFloat(),
                                        componentData[ORIGIN_KEY][1].asFloat(),
                                        componentData[ORIGIN_KEY][2].asFloat());

                            Vec3 scale(componentData[SCALE_KEY][0].asFloat(),
                                       componentData[SCALE_KEY][1].asFloat(),
                                       componentData[SCALE_KEY][2].asFloat());

                            Quaternion rotation(componentData[ROTATION_KEY][0].asFloat(),
                                                componentData[ROTATION_KEY][1].asFloat(),
                                                componentData[ROTATION_KEY][2].asFloat(),
                                                componentData[ROTATION_KEY][3].asFloat());

                            transform->SetOrigin(origin);
                            transform->SetScale(scale);
                            transform->SetRotation(rotation);
                            transform->Update();

                        } else if (componentType == "mesh") {
                            hd::graphics::MeshPrototype prototype;
                            prototype.meshMethod = (hd::graphics::MeshMethod)componentData[MESH_METHOD_KEY].asInt();
                            prototype.cylinderRadius = componentData[CYLINDER_RADIUS_KEY].asFloat();
                            prototype.cylinderLength = componentData[CYLINDER_LENGTH_KEY].asFloat();
                            prototype.cylinderSegments = componentData[CYLINDER_SEGMENTS_KEY].asFloat();

                            prototype.quadAABB.lower = Vec3(componentData[QUAD_AABB_KEY][0].asFloat(),
                                                            componentData[QUAD_AABB_KEY][1].asFloat(),
                                                            componentData[QUAD_AABB_KEY][2].asFloat());
                            prototype.quadAABB.upper = Vec3(componentData[QUAD_AABB_KEY][3].asFloat(),
                                                            componentData[QUAD_AABB_KEY][4].asFloat(),
                                                            componentData[QUAD_AABB_KEY][5].asFloat());

                            prototype.cubeAABB.lower = Vec3(componentData[CUBE_AABB_KEY][0].asFloat(),
                                                            componentData[CUBE_AABB_KEY][1].asFloat(),
                                                            componentData[CUBE_AABB_KEY][2].asFloat());
                            prototype.cubeAABB.upper = Vec3(componentData[CUBE_AABB_KEY][3].asFloat(),
                                                            componentData[CUBE_AABB_KEY][4].asFloat(),
                                                            componentData[CUBE_AABB_KEY][5].asFloat());

                            prototype.sphereRadius = componentData[SPHERE_RADIUS_KEY].asFloat();
                            prototype.sphereSegments = componentData[SPHERE_SEGMENTS_KEY].asFloat();

                            prototype.repeatingGridAABB.lower = Vec3(componentData[REPEATING_GRID_AABB_KEY][0].asFloat(),
                                                                     componentData[REPEATING_GRID_AABB_KEY][1].asFloat(),
                                                                     componentData[REPEATING_GRID_AABB_KEY][2].asFloat());
                            prototype.repeatingGridAABB.upper = Vec3(componentData[REPEATING_GRID_AABB_KEY][3].asFloat(),
                                                                     componentData[REPEATING_GRID_AABB_KEY][4].asFloat(),
                                                                     componentData[REPEATING_GRID_AABB_KEY][5].asFloat());

                            prototype.repeatingGridCubeSize = Size3f(componentData[REPEATING_GRID_CUBE_SIZE_KEY][0].asFloat(),
                                                                    componentData[REPEATING_GRID_CUBE_SIZE_KEY][1].asFloat(),
                                                                    componentData[REPEATING_GRID_CUBE_SIZE_KEY][2].asFloat());
                            prototype.repeatingGridGapSize = Size3f(componentData[REPEATING_GRID_GAP_SIZE_KEY][0].asFloat(),
                                                                   componentData[REPEATING_GRID_GAP_SIZE_KEY][1].asFloat(),
                                                                   componentData[REPEATING_GRID_GAP_SIZE_KEY][2].asFloat());

                            prototype.boxWithCurvedCornersSize = Size3f(componentData[REPEATING_GRID_GAP_SIZE_KEY][0].asFloat(),
                                                                       componentData[REPEATING_GRID_GAP_SIZE_KEY][1].asFloat(),
                                                                       componentData[REPEATING_GRID_GAP_SIZE_KEY][2].asFloat());
                            prototype.boxWithCurvedCornersCornerRadius = componentData[BOX_WITH_CURVED_CORNERS_RADIUS_KEY].asFloat();

                            prototype.gapGridSize = Vec3();
                            prototype.gapGridNodeSize = Vec3();

                            prototype.meshFilePath = componentData[MESH_PATH_KEY].asString();

                            shader->LoadShader("blinnPhongGeometryInstanced.vert", "blinnPhongTexture.frag");

                            auto meshData = hd::graphics::MeshLoader::Instance()->GetMesh(prototype);
                            mesh->SetMeshReference(meshData);
                            mesh->GetMeshData()->SetDefaultAttributeLocations(shader);
                            mesh->GetMeshData()->SetInstancedAttributeLocations(shader);
                            collisionData->SetMeshData(meshData);
                            opacityData->SetOpacity(1.0f);

                        } else if (componentType == "material") {
                            if (componentData.isMember(MATERIAL_TEXTURE_KEY)) {
                                auto texture = componentData[MATERIAL_TEXTURE_KEY].asString();
                                material->SetTexture(hd::TextureManager::Instance()->FindTexture(texture.c_str()));
                            } else {
                                shader->LoadShader("blinnPhongGeometryInstanced.vert", "blinnPhongGeometry.frag");
                            }

                            if (componentData.isMember(MATERIAL_AMBIENT_COLOR_KEY)) {
                                Color4f ambient = Color4f(componentData[MATERIAL_AMBIENT_COLOR_KEY][0].asFloat(),
                                                          componentData[MATERIAL_AMBIENT_COLOR_KEY][1].asFloat(),
                                                          componentData[MATERIAL_AMBIENT_COLOR_KEY][2].asFloat(),
                                                          componentData[MATERIAL_AMBIENT_COLOR_KEY][3].asFloat());

                                material->SetAmbient(ambient);
                            }

                            if (componentData.isMember(MATERIAL_DIFFUSE_COLOR_KEY)) {
                                Color4f diffuse = Color4f(componentData[MATERIAL_DIFFUSE_COLOR_KEY][0].asFloat(),
                                                          componentData[MATERIAL_DIFFUSE_COLOR_KEY][1].asFloat(),
                                                          componentData[MATERIAL_DIFFUSE_COLOR_KEY][2].asFloat(),
                                                          componentData[MATERIAL_DIFFUSE_COLOR_KEY][3].asFloat());
                                material->SetDiffuse(diffuse);
                            }

                            if (componentData.isMember(MATERIAL_SPECULAR_COLOR_KEY)) {
                                Color4f specular = Color4f(componentData[MATERIAL_SPECULAR_COLOR_KEY][0].asFloat(),
                                                           componentData[MATERIAL_SPECULAR_COLOR_KEY][1].asFloat(),
                                                           componentData[MATERIAL_SPECULAR_COLOR_KEY][2].asFloat(),
                                                           componentData[MATERIAL_SPECULAR_COLOR_KEY][3].asFloat());
                                material->SetSpecular(specular);
                            }

                            if (componentData.isMember(MATERIAL_EMISSION_COLOR_KEY)) {
                                Color4f emission = Color4f(componentData[MATERIAL_EMISSION_COLOR_KEY][0].asFloat(),
                                                           componentData[MATERIAL_EMISSION_COLOR_KEY][1].asFloat(),
                                                           componentData[MATERIAL_EMISSION_COLOR_KEY][2].asFloat(),
                                                           componentData[MATERIAL_EMISSION_COLOR_KEY][3].asFloat());
                                material->SetEmission(emission);
                            }

                            if (componentData.isMember(MATERIAL_AMBIENT_ADJUSTMENT_KEY)) {
                                Color4f ambientAdj = Color4f(componentData[MATERIAL_AMBIENT_ADJUSTMENT_KEY][0].asFloat(),
                                                             componentData[MATERIAL_AMBIENT_ADJUSTMENT_KEY][1].asFloat(),
                                                             componentData[MATERIAL_AMBIENT_ADJUSTMENT_KEY][2].asFloat(),
                                                             componentData[MATERIAL_AMBIENT_ADJUSTMENT_KEY][3].asFloat());
                                material->SetAmbientAdjustment(ambientAdj);
                            }
                            
                            float shininess = componentData[MATERIAL_SHININESS_KEY].asFloat();
                            material->SetShininess(shininess);
                        }
                    }
                }
                
                if (onRestoreCallback) {
                    onRestoreCallback(newItem);
                }
            }
        }
    }
}
