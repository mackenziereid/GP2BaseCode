#include "FBXLoader.h"

int level = 0;

void PrintTabs()
{
    for (int i = 0; 1 < level; i++) {
        printf("\t");
    }
}


//Return a string-based representation based on the arrtibute type
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type)
{
    switch (type) {
        case FbxNodeAttribute::eUnknown: return "unidentified";
        case FbxNodeAttribute::eNull: return "null";
        case FbxNodeAttribute::eMarker: return "marker";
        case FbxNodeAttribute::eSkeleton: return "Skeleton";
        case FbxNodeAttribute::eMesh: return "mesh";
        case FbxNodeAttribute::eNurbs: return "nurbs";
        case FbxNodeAttribute::ePatch: return "patch";
        case FbxNodeAttribute::eCamera: return "camera";
        case FbxNodeAttribute::eCameraStereo: return "stereo";
        case FbxNodeAttribute::eCameraSwitcher: return "switcher";
        case FbxNodeAttribute::eLight: return "light";
        case FbxNodeAttribute::eOpticalReference: return "optical reference";
        case FbxNodeAttribute::eOpticalMarker: return "marker";
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
        case FbxNodeAttribute::eBoundary: return "boundary";
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
        case FbxNodeAttribute::eShape: return "shape";
        case FbxNodeAttribute::eLODGroup: return "lodgroup";
        case FbxNodeAttribute::eSubDiv: return "subdiv";
        default: return "unknown";
    }
}

bool loadFBXFromFile(const string& filename, MeshData *meshData)
{
    
    level = 0;
    //Initialise the SDK manager. This object handles memory management.
    FbxManager* ISdkManager = FbxManager::Create();
    
    //Create the IO settings object
    FbxIOSettings *ios = FbxIOSettings::Create(ISdkManager, IOSROOT);
    ISdkManager->SetIOSettings(ios);
    
    //create an importer using the SDK manager
    FbxImporter* IImporter = FbxImporter::Create(ISdkManager, "");
    
    //Call the initalise using the SDK manager
    if(IImporter->Initialize(filename.c_str(), -1, ISdkManager->GetIOSettings()))
    {
        return false;
    }
    
    //creates a new scene so that it can be populated by the imported file.
    FbxScene* IScene = FbxScene::Create(ISdkManager, "myScene");
    //import the contents of the file into the scene
    IImporter->Import(IScene);
    
    //Process nodes
    FbxNode* IRootNode =IScene->GetRootNode();
    if (IRootNode) {
        cout << "Root Node " << IRootNode->GetName() << endl;
        for (int i = 0; i < IRootNode->GetChildCount(); i++) {
            processNode(IRootNode->GetChild(i), meshData);
        }
    }
    
    IImporter->Destroy();
    return true;
}

void processNode(FbxNode * node, MeshData *meshData)
{
    PrintTabs();
    const char* nodeName = node->GetName();
    FbxDouble3 translation = node->LclTranslation.Get();
    FbxDouble3 rotation = node->LclRotation.Get();
    FbxDouble3 scaling = node->LclScaling.Get();
    
    cout << "Node" << nodeName
         << " Position " << translation[0] << " " << translation[1] << " " << translation[2] << " "
         << " Rotation " << rotation[0] << " " << rotation[1] << " " << rotation[2] << " "
         << " Scale " << scaling[0] << " " << scaling[1] << " " << scaling[2] << endl;
    
    level++;
    //print the nodes attributes
    for (int i = 0; node->GetNodeAttributeCount(); i++) {
        processNodeAttribute(node->GetNodeAttributeByIndex(i), meshData);
    }
    
    //Recursice print the children
    for (int j = 0; node->GetChildCount(); j++) {
        processNode(node->GetChild(j), meshData);
    }
    
    level--;
    PrintTabs();
}

void processNodeAttribute(FbxNodeAttribute * attribute, MeshData *meshData)
{
    if(!attribute) return;
    FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
    FbxString attrName = attribute->GetName();
    PrintTabs();
    cout << "Attribute " << typeName.Buffer() << " Name " << attrName << endl;
    switch (attribute->GetAttributeType()) {
        case FbxNodeAttribute::eMesh: processMesh(attribute->GetNode()->GetMesh(), meshData);
        case FbxNodeAttribute::eCamera: return;
        case FbxNodeAttribute::eLight: return;
    }
}

void processMesh(FbxMesh * mesh, MeshData *meshData)
{
    int numVerts = mesh->GetControlPointsCount();
    int numIndices = mesh->GetPolygonVertexCount();
    
    Vertex * pVerts = new Vertex[numVerts];
    int * pIndices = mesh->GetPolygonVertices();
    
    for (int i = 0; i < numVerts; i++) {
        FbxVector4 currentVert = mesh->GetControlPointAt(i);
        pVerts[i].position = vec3(currentVert[0], currentVert[1], currentVert[2]);
        pVerts[i].colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        pVerts[i].texCoords = vec2(0.0f, 0.0f);
    }
    
    processMeshTextureCoords(mesh,pVerts, numVerts);
    
    for (int i = 0; i < numVerts; i ++) {
        meshData->vertices.push_back(pVerts[i]);
    }
    for (int i = 0; i < numIndices; i++) {
        meshData->indices.push_back(pIndices[i]);
    }
    cout << "Vertices " << numVerts << "Indices " << numIndices << endl;
    
    if(pVerts)
    {
        delete[] pVerts;
        pVerts = NULL;
    }
}

void processMeshTextureCoords(FbxMesh * mesh, Vertex * verts, int numVerts)
{
    for (int IPolygon = 0; IPolygon < mesh->GetPolygonCount(); IPolygon++) {
        for (unsigned int IpolygonVertex=0; IpolygonVertex < 3; IpolygonVertex++) {
            int fbxCornerIndex = mesh->GetPolygonVertex(IPolygon, IpolygonVertex);
            FbxVector2 fbxUV = FbxVector2(0.0,0.0);
            FbxLayerElementUV* fbxLayerUV = mesh->GetLayer(0)->GetUVs();
            //Get Texture coordinates
            if(fbxLayerUV)
            {
                int iUVIndex = 0;
                switch (fbxLayerUV->GetMappingMode()) {
                    case FbxLayerElement::eByPolygonVertex: iUVIndex = mesh->GetTextureUVIndex(IPolygon, IpolygonVertex, FbxLayerElement::eTextureDiffuse);
                        break;
                    case FbxLayerElement::eByPolygon: iUVIndex = IPolygon;
                        break;
                }
                
                fbxUV = fbxLayerUV->GetDirectArray().GetAt(iUVIndex);
                verts[fbxCornerIndex].texCoords.x = fbxUV[0];
                verts[fbxCornerIndex].texCoords.y = 1.0f - fbxUV[1];
                
            }
        }
    }
}
