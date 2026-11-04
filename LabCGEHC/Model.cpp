#include "Model.h"
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <filesystem>
#include <algorithm>


Model::Model()
{
}

void Model::LoadModel(const std::string & fileName)
{
	directorioModelo = std::filesystem::path(fileName).parent_path().string();
	Assimp::Importer importer;//					Pasa de Polygons y Quads a triangulos, modifica orden para el origen, generar normales si el  objeto no tiene, trata vértices iguales como 1 solo
	//const aiScene *scene=importer.ReadFile(fileName,aiProcess_Triangulate |aiProcess_FlipUVs|aiProcess_GenSmoothNormals|aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);
	if (!scene)
	{	
		printf("Falló en cargar el modelo: %s \n", fileName.c_str(), importer.GetErrorString());
		return;
	}
    minimo = aiVector3D(1e30f); maximo = aiVector3D(-1e30f);
    for (unsigned m=0; m<scene->mNumMeshes; ++m)
        for (unsigned v=0; v<scene->mMeshes[m]->mNumVertices; ++v) {
            const auto& p=scene->mMeshes[m]->mVertices[v];
            for(int k=0;k<3;++k) { minimo[k]=(std::min)(minimo[k],p[k]); maximo[k]=(std::max)(maximo[k],p[k]); }
        }
	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
	}

void Model::ClearModel()
{
	for (unsigned int i = 0; i < MeshList.size(); i++)
	{
		if (MeshList[i])
		{
			delete MeshList[i];
			MeshList[i] = nullptr;

		}
	}

	std::unordered_set<Texture*> liberadas;
	for (unsigned int i = 0; i < TextureList.size(); i++)
	{
		if (TextureList[i])
		{
			if (liberadas.insert(TextureList[i]).second) delete TextureList[i];
			TextureList[i] = nullptr;
		}
	}

}

void Model::RenderModel()
{
	for (unsigned int i = 0; i < MeshList.size(); i++)
	{
		unsigned int materialIndex = meshTotex[i];
		if (materialIndex < TextureList.size() && TextureList[materialIndex])
		{
			TextureList[materialIndex]->UseTexture();
		}
		MeshList[i]->RenderMeshModel();

	}


}


Model::~Model()
{
}

void Model::LoadNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i <node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh * mesh, const aiScene * scene)
{

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z });
		//UV
		if (mesh->mTextureCoords[0])//si tiene coordenadas de texturizado
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y});
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f,0.0f });
		}
		//Normals importante, las normales son negativas porque la luz interactúa con ellas de esa forma, cómo se vio con el dado/cubo
		
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,-mesh->mNormals[i].y ,-mesh->mNormals[i].z });
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	MeshModel* newMesh = new MeshModel();
	newMesh->CreateMeshModel(&vertices[0], &indices[0], vertices.size(), indices.size());
	MeshList.push_back(newMesh);
	meshTotex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
    std::unordered_map<std::string, Texture*> loadedTextures;
    TextureList.resize(scene->mNumMaterials, nullptr);
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial* material = scene->mMaterials[i];
        aiString path;
        // glTF usa BASE_COLOR; los OBJ conservan DIFFUSE como alternativa.
        bool hasTexture = material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS;
        if (!hasTexture) hasTexture = material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS;
        TextureList[i] = nullptr;
        if (hasTexture) {
            const aiTexture* embedded = scene->GetEmbeddedTexture(path.C_Str());
            std::string name = path.C_Str();
            std::size_t slash = name.find_last_of("/\\");
            std::string filename = slash == std::string::npos ? name : name.substr(slash + 1);
            std::string texturePath = "Textures/" + filename;
            if (!embedded) {
                for (const auto& candidate : {directorioModelo + "/" + name,
                     directorioModelo + "/" + filename,
                     (directorioModelo.find("Equipo") != std::string::npos ? "Textures/Equipo/" : "Textures/") + filename,
                     "Textures/" + filename}) {
                    if (std::filesystem::exists(candidate)) { texturePath=candidate; break; }
                }
            }
            std::string key = embedded ? "embedded:" + name : texturePath;
            auto found = loadedTextures.find(key);
            if (found != loadedTextures.end()) TextureList[i] = found->second;
            else {
                Texture* texture = embedded ? new Texture() : new Texture(key.c_str());
                bool loaded = false;
                if (embedded) {
                    if (embedded->mHeight == 0 && embedded->mWidth <= static_cast<unsigned int>((std::numeric_limits<int>::max)())) {
                        // mWidth representa bytes cuando la imagen esta comprimida.
                        loaded = texture->LoadFromMemory(reinterpret_cast<const unsigned char*>(embedded->pcData), static_cast<int>(embedded->mWidth));
                    } else if (embedded->mHeight > 0) {
                        // aiTexel es BGRA: convertir a RGBA y voltear filas.
                        std::vector<unsigned char> rgba(static_cast<size_t>(embedded->mWidth) * embedded->mHeight * 4);
                        for (unsigned int y = 0; y < embedded->mHeight; ++y) {
                            for (unsigned int x = 0; x < embedded->mWidth; ++x) {
                                const aiTexel& p = embedded->pcData[(embedded->mHeight - 1 - y) * embedded->mWidth + x];
                                size_t k = (static_cast<size_t>(y) * embedded->mWidth + x) * 4;
                                rgba[k] = p.r; rgba[k+1] = p.g; rgba[k+2] = p.b; rgba[k+3] = p.a;
                            }
                        }
                        loaded = texture->LoadRGBA(rgba.data(), embedded->mWidth, embedded->mHeight);
                    }
                } else {
                    // Mantener las rutas y los metodos de carga de los OBJ existentes.
                    std::string ext = filename.substr(filename.find_last_of('.') + 1);
                    loaded = (ext == "png" || ext == "tga") ? texture->LoadTextureA() : texture->LoadTexture();
                }
                if (loaded) { TextureList[i] = texture; loadedTextures[key] = texture; }
                else { printf("No se pudo cargar textura: %s\n", key.c_str()); delete texture; }
            }
        }
        if (!TextureList[i]) {
            aiColor4D diffuse(1,1,1,1);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
            unsigned char rgba[4];
            for(int k=0;k<3;++k) rgba[k]=static_cast<unsigned char>(255.0f*std::clamp(diffuse[k],0.0f,1.0f));
            rgba[3]=255;
            TextureList[i] = new Texture();
            TextureList[i]->LoadRGBA(rgba,1,1);
        }
    }
}
