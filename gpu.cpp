/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <array>





/// \addtogroup gpu_init
/// @{

/**
 * @brief Constructor of GPU
 */
GPU::GPU(){
  /// \todo Zde můžete alokovat/inicializovat potřebné proměnné grafické karty
}

/**
 * @brief Destructor of GPU
 */
GPU::~GPU(){
  /// \todo Zde můžete dealokovat/deinicializovat grafickou kartu
}

/// @}

/** \addtogroup buffer_tasks 01. Implementace obslužných funkcí pro buffery
 * @{
 */

/**
 * @brief This function allocates buffer on GPU.
 *
 * @param size size in bytes of new buffer on GPU.
 *
 * @return unique identificator of the buffer
 */
BufferID GPU::createBuffer(uint64_t size) { 
  /// \todo Tato funkce by měla na grafické kartě vytvořit buffer dat.<br>
  /// Velikost bufferu je v parameteru size (v bajtech).<br>
  /// Funkce by měla vrátit unikátní identifikátor identifikátor bufferu.<br>
  /// Na grafické kartě by mělo být možné alkovat libovolné množství bufferů o libovolné velikosti.<br>
    auto buffer = std::vector<uint8_t>(size_t(size));
    BufferID id = (BufferID)buffer.data();
    buffers.try_emplace(id, std::move(buffer));
    return id;    
}

/**
 * @brief This function frees allocated buffer on GPU.
 *
 * @param buffer buffer identificator
 */
void GPU::deleteBuffer(BufferID buffer) {
  /// \todo Tato funkce uvolní buffer na grafické kartě.
  /// Buffer pro smazání je vybrán identifikátorem v parameteru "buffer".
  /// Po uvolnění bufferu je identifikátor volný a může být znovu použit při vytvoření nového bufferu.
    buffers.erase(buffer);
}

/**
 * @brief This function uploads data to selected buffer on the GPU
 *
 * @param buffer buffer identificator
 * @param offset specifies the offset into the buffer's data
 * @param size specifies the size of buffer that will be uploaded
 * @param data specifies a pointer to new data
 */
void GPU::setBufferData(BufferID buffer, uint64_t offset, uint64_t size, void const* data) {
  /// \todo Tato funkce nakopíruje data z cpu na "gpu".<br>
  /// Data by měla být nakopírována do bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje, kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) kam se data nakopírují.<br>
  /// Parametr data obsahuje ukazatel na data na cpu pro kopírování.<br>
    auto mybuffer = buffers.find(buffer);
    std::copy((uint8_t*) data, (uint8_t*) data + size, mybuffer->second.begin() + offset);
}

/**
 * @brief This function downloads data from GPU.
 *
 * @param buffer specfies buffer
 * @param offset specifies the offset into the buffer from which data will be returned, measured in bytes. 
 * @param size specifies data size that will be copied
 * @param data specifies a pointer to the location where buffer data is returned. 
 */
void GPU::getBufferData(BufferID buffer, uint64_t offset, uint64_t size, void* data)
{
  /// \todo Tato funkce vykopíruje data z "gpu" na cpu.
  /// Data by měla být vykopírována z bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) odkud se začne kopírovat.<br>
  /// Parametr data obsahuje ukazatel, kam se data nakopírují.<br>
    /*auto mybuffer = buffers.find(buffer);
    std::copy(mybuffer->second.begin() + offset, mybuffer->second.begin() + offset + size, (uint8_t*)data);*/
    std::copy(buffers.find(buffer)->second.begin() + offset, buffers.find(buffer)->second.begin() + offset + size, (uint8_t*)data);
}

/**
 * @brief This function tests if buffer exists
 *
 * @param buffer selected buffer id
 *
 * @return true if buffer points to existing buffer on the GPU.
 */
bool GPU::isBuffer(BufferID buffer) { 
  /// \todo Tato funkce by měla vrátit true pokud buffer je identifikátor existující bufferu.<br>
  /// Tato funkce by měla vrátit false, pokud buffer není identifikátor existujícího bufferu. (nebo bufferu, který byl smazán).<br>
  /// Pro emptyId vrací false.<br>
    auto bufptr = buffers.find(buffer);
    if (bufptr != buffers.end())
        return true;
    else
        return false; 
}

/// @}

/**
 * \addtogroup vertexpuller_tasks 02. Implementace obslužných funkcí pro vertex puller
 * @{
 */

/**
 * @brief This function creates new vertex puller settings on the GPU,
 *
 * @return unique vertex puller identificator
 */
ObjectID GPU::createVertexPuller     (){
  /// \todo Tato funkce vytvoří novou práznou tabulku s nastavením pro vertex puller.<br>
  /// Funkce by měla vrátit identifikátor nové tabulky.
  /// Prázdná tabulka s nastavením neobsahuje indexování a všechny čtecí hlavy jsou vypnuté.
    //auto vertexpuller = std::vector<VertexPuller>(sizeof(VertexPuller));                        
    auto vertexpuller = std::make_unique<VertexPuller>();
    //VertexPullerID id = (VertexPullerID)vertexpuller.data();                             
    VertexPullerID id = (VertexPullerID)vertexpuller.get();
    vertexPullers.try_emplace(id, std::move(vertexpuller));
    return id;
}

/**
 * @brief This function deletes vertex puller settings
 *
 * @param vao vertex puller identificator
 */
void     GPU::deleteVertexPuller     (VertexPullerID vao){
  /// \todo Tato funkce by měla odstranit tabulku s nastavení pro vertex puller.<br>
  /// Parameter "vao" obsahuje identifikátor tabulky s nastavením.<br>
  /// Po uvolnění nastavení je identifiktátor volný a může být znovu použit.<br>
    vertexPullers.erase(vao);
}

/**
 * @brief This function sets one vertex puller reading head.
 *
 * @param vao identificator of vertex puller
 * @param head id of vertex puller head
 * @param type type of attribute
 * @param stride stride in bytes
 * @param offset offset in bytes
 * @param buffer id of buffer
 */
void GPU::setVertexPullerHead(VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer){
  /// \todo Tato funkce nastaví jednu čtecí hlavu vertex pulleru.<br>
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "head" vybírá čtecí hlavu vybraného vertex pulleru.<br>
  /// Parametr "type" nastaví typ atributu, který čtecí hlava čte. Tímto se vybere kolik dat v bajtech se přečte.<br>
  /// Parametr "stride" nastaví krok čtecí hlavy.<br>
  /// Parametr "offset" nastaví počáteční pozici čtecí hlavy.<br>
  /// Parametr "buffer" vybere buffer, ze kterého bude čtecí hlava číst.<br>
    auto vertexpuller = vertexPullers.find(vao);
    vertexpuller->second->Headsettings[head].currentBuffer = buffer;
    vertexpuller->second->Headsettings[head].offset = offset;
    vertexpuller->second->Headsettings[head].stride = stride;
    vertexpuller->second->Headsettings[head].type = type;
}

/**
 * @brief This function sets vertex puller indexing.
 *
 * @param vao vertex puller id
 * @param type type of index
 * @param buffer buffer with indices
 */
void     GPU::setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer){
  /// \todo Tato funkce nastaví indexování vertex pulleru.
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "type" volí typ indexu, který je uložený v bufferu.<br>
  /// Parametr "buffer" volí buffer, ve kterém jsou uloženy indexy.<br>
    auto vertexpuller = vertexPullers.find(vao);
    vertexpuller->second->indexing.currentBuffer = buffer;
    vertexpuller->second->indexing.type = type;
}

/**
 * @brief This function enables vertex puller's head.
 *
 * @param vao vertex puller 
 * @param head head id
 */
void     GPU::enableVertexPullerHead (VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce povolí čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava povolena, hodnoty z bufferu se budou kopírovat do atributu vrcholů vertex shaderu.<br>
  /// Parametr "vao" volí tabulku s nastavením vertex pulleru (vybírá vertex puller).<br>
  /// Parametr "head" volí čtecí hlavu.<br>
   /* auto vertexpuller = vertexPullers.find(vao);
    vertexpuller->second->Headsettings[head].Enabled = true;*/
    vertexPullers.find(vao)->second->Headsettings[head].Enabled = true;
}

/**
 * @brief This function disables vertex puller's head
 *
 * @param vao vertex puller id
 * @param head head id
 */
void     GPU::disableVertexPullerHead(VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce zakáže čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava zakázána, hodnoty z bufferu se nebudou kopírovat do atributu vrcholu.<br>
  /// Parametry "vao" a "head" vybírají vertex puller a čtecí hlavu.<br>
   /* auto vertexpuller = vertexPullers.find(vao);
    vertexpuller->second->Headsettings[head].Enabled = false;*/
    vertexPullers.find(vao)->second->Headsettings[head].Enabled = false;
}

/**
 * @brief This function selects active vertex puller.
 *
 * @param vao id of vertex puller
 */
void     GPU::bindVertexPuller       (VertexPullerID vao){
  /// \todo Tato funkce aktivuje nastavení vertex pulleru.<br>
  /// Pokud je daný vertex puller aktivován, atributy z bufferů jsou vybírány na základě jeho nastavení.<br>
    vertexpullerActive = vertexPullers.find(vao)->second.get();
}

/**
 * @brief This function deactivates vertex puller.
 */
void     GPU::unbindVertexPuller     (){
  /// \todo Tato funkce deaktivuje vertex puller.
  /// To většinou znamená, že se vybere neexistující "emptyID" vertex puller.
    vertexpullerActive = nullptr;
}

/**
 * @brief This function tests if vertex puller exists.
 *
 * @param vao vertex puller
 *
 * @return true, if vertex puller "vao" exists
 */
bool     GPU::isVertexPuller         (VertexPullerID vao){
  /// \todo Tato funkce otestuje, zda daný vertex puller existuje.
  /// Pokud ano, funkce vrací true.
    auto vertexpuller = vertexPullers.find(vao);
    if (vertexpuller != vertexPullers.end())
        return true;
    else
        return false;
}

/// @}

/** \addtogroup program_tasks 03. Implementace obslužných funkcí pro shader programy
 * @{
 */

/**
 * @brief This function creates new shader program.
 *
 * @return shader program id
 */
ProgramID        GPU::createProgram         (){
  /// \todo Tato funkce by měla vytvořit nový shader program.<br>
  /// Funkce vrací unikátní identifikátor nového proramu.<br>
  /// Program je seznam nastavení, které obsahuje: ukazatel na vertex a fragment shader.<br>
  /// Dále obsahuje uniformní proměnné a typ výstupních vertex attributů z vertex shaderu, které jsou použity pro interpolaci do fragment atributů.<br>
    auto program = std::make_unique<ProgramSettings>();
    ProgramID id = (ProgramID)program.get();
    programs.try_emplace(id, std::move(program));
    return id;
}

/**
 * @brief This function deletes shader program
 *
 * @param prg shader program id
 */
void GPU::deleteProgram (ProgramID prg){
  /// \todo Tato funkce by měla smazat vybraný shader program.<br>
  /// Funkce smaže nastavení shader programu.<br>
  /// Identifikátor programu se stane volným a může být znovu využit.<br>
    programs.erase(prg);
}

/**
 * @brief This function attaches vertex and frament shader to shader program.
 *
 * @param prg shader program
 * @param vs vertex shader 
 * @param fs fragment shader
 */
void             GPU::attachShaders         (ProgramID prg,VertexShader vs,FragmentShader fs){
  /// \todo Tato funkce by měla připojít k vybranému shader programu vertex a fragment shader.
    programs.find(prg)->second->VertexShader = vs;
    programs.find(prg)->second->FragmentShader = fs;
}

/**
 * @brief This function selects which vertex attributes should be interpolated during rasterization into fragment attributes.
 *
 * @param prg shader program
 * @param attrib id of attribute
 * @param type type of attribute
 */
void             GPU::setVS2FSType          (ProgramID prg,uint32_t attrib,AttributeType type){
  /// \todo tato funkce by měla zvolit typ vertex atributu, který je posílán z vertex shaderu do fragment shaderu.<br>
  /// V průběhu rasterizace vznikají fragment.<br>
  /// Fragment obsahují fragment atributy.<br>
  /// Tyto atributy obsahují interpolované hodnoty vertex atributů.<br>
  /// Tato funkce vybere jakého typu jsou tyto interpolované atributy.<br>
  /// Bez jakéhokoliv nastavení jsou atributy prázdne AttributeType::EMPTY<br>
    programs.find(prg)->second->Attributes[attrib] = type;
}

/**
 * @brief This function actives selected shader program
 *
 * @param prg shader program id
 */
void             GPU::useProgram            (ProgramID prg){
  /// \todo tato funkce by měla vybrat aktivní shader program.
    programActive = programs.find(prg)->second.get();
}

/**
 * @brief This function tests if selected shader program exists.
 *
 * @param prg shader program
 *
 * @return true, if shader program "prg" exists.
 */
bool             GPU::isProgram             (ProgramID prg){
  /// \todo tato funkce by měla zjistit, zda daný program existuje.<br>
  /// Funkce vráti true, pokud program existuje.<br>
    auto program = programs.find(prg);
    if (program != programs.end())
        return true;
    else
        return false;
}

/**
 * @brief This function sets uniform value (1 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform1f      (ProgramID prg,uint32_t uniformId,float const&d){
  /// \todo tato funkce by měla nastavit uniformní proměnnou shader programu.<br>
  /// Parametr "prg" vybírá shader program.<br>
  /// Parametr "uniformId" vybírá uniformní proměnnou. Maximální počet uniformních proměnných je uložen v programné \link maxUniforms \endlink.<br>
  /// Parametr "d" obsahuje data (1 float).<br>
    programs.find(prg)->second->Uniforms.uniform[uniformId].v1 = d;
}

/**
 * @brief This function sets uniform value (2 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform2f      (ProgramID prg,uint32_t uniformId,glm::vec2 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 2 floaty.
    programs.find(prg)->second->Uniforms.uniform[uniformId].v2 = d;
}

/**
 * @brief This function sets uniform value (3 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform3f      (ProgramID prg,uint32_t uniformId,glm::vec3 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 3 floaty.
    programs.find(prg)->second->Uniforms.uniform[uniformId].v3 = d;
}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform4f      (ProgramID prg,uint32_t uniformId,glm::vec4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 4 floaty.
    programs.find(prg)->second->Uniforms.uniform[uniformId].v4 = d;
}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniformMatrix4f(ProgramID prg,uint32_t uniformId,glm::mat4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává matici 4x4 (16 floatů).
    programs.find(prg)->second->Uniforms.uniform[uniformId].m4 = d;
}

/// @}





/** \addtogroup framebuffer_tasks 04. Implementace obslužných funkcí pro framebuffer
 * @{
 */

/**
 * @brief This function creates framebuffer on GPU.
 *
 * @param width width of framebuffer
 * @param height height of framebuffer
 */
void GPU::createFramebuffer      (uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla alokovat framebuffer od daném rozlišení.<br>
  /// Framebuffer se skládá z barevného a hloukového bufferu.<br>
  /// Buffery obsahují width x height pixelů.<br>
  /// Barevný pixel je složen z 4 x uint8_t hodnot - to reprezentuje RGBA barvu.<br>
  /// Hloubkový pixel obsahuje 1 x float - to reprezentuje hloubku.<br>
  /// Nultý pixel framebufferu je vlevo dole.<br>
    Width = width;
    Height = height;
    Framebuffer.resize(width * height);
    depth.resize(width * height);
}

/**
 * @brief This function deletes framebuffer.
 */
void GPU::deleteFramebuffer      (){
  /// \todo tato funkce by měla dealokovat framebuffer.
    {}
}

/**
 * @brief This function resizes framebuffer.
 *
 * @param width new width of framebuffer
 * @param height new heght of framebuffer
 */
void     GPU::resizeFramebuffer(uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla změnit velikost framebuffer.
    createFramebuffer(width, height);
}

/**
 * @brief This function returns pointer to color buffer.
 *
 * @return pointer to color buffer
 */
uint8_t* GPU::getFramebufferColor  (){
  /// \todo Tato funkce by měla vrátit ukazatel na začátek barevného bufferu.<br>
    return &Framebuffer[0].R;

}

/**
 * @brief This function returns pointer to depth buffer.
 *
 * @return pointer to dept buffer.
 */
float* GPU::getFramebufferDepth    (){
  /// \todo tato funkce by mla vrátit ukazatel na začátek hloubkového bufferu.<br>
    return &depth[0];
}

/**
 * @brief This function returns width of framebuffer
 *
 * @return width of framebuffer
 */
uint32_t GPU::getFramebufferWidth (){
  /// \todo Tato funkce by měla vrátit šířku framebufferu.
    return Width;
}

/**
 * @brief This function returns height of framebuffer.
 *
 * @return height of framebuffer
 */
uint32_t GPU::getFramebufferHeight(){
  /// \todo Tato funkce by měla vrátit výšku framebufferu.
    return Height;
}

/// @}

/** \addtogroup draw_tasks 05. Implementace vykreslovacích funkcí
 * Bližší informace jsou uvedeny na hlavní stránce dokumentace.
 * @{
 */

/**
 * @brief This functino clears framebuffer.
 *
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void  GPU::clear (float r,float g,float b,float a){
  /// \todo Tato funkce by měla vyčistit framebuffer.<br>
  /// Barevný buffer vyčistí na barvu podle parametrů r g b a (0 - nulová intenzita, 1 a větší - maximální intenzita).<br>
  /// (0,0,0) - černá barva, (1,1,1) - bílá barva.<br>
  /// Hloubkový buffer nastaví na takovou hodnotu, která umožní rasterizaci trojúhelníka, který leží v rámci pohledového tělesa.<br>
  /// Hloubka by měla být tedy větší než maximální hloubka v NDC (normalized device coordinates).<br>
    //std::fill(Framebuffer.begin(), Framebuffer.end(), ColorBuffer{ (uint8_t)r,(uint8_t)g,(uint8_t)b,(uint8_t)a });
    //std::fill(depth.begin(), depth.end(), 2.0f);

    for (size_t i = 0; i < Framebuffer.size(); i++)
    {
        Framebuffer[i].R = (uint8_t)(r * 255.0f);
        Framebuffer[i].G = (uint8_t)(g * 255.0f);
        Framebuffer[i].B = (uint8_t)(b * 255.0f);
        Framebuffer[i].A = (uint8_t)(a * 255.0f);
    }

    for (size_t j = 0; j < depth.size(); j++)
    {
        depth[j] = 2.0f;
    }
}



void            GPU::drawTriangles(uint32_t  nofVertices) 
{
    /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
    /// Vrcholy se budou vybírat podle nastavení z aktivního vertex pulleru (pomocí bindVertexPuller).<br>
    /// Vertex shader a fragment shader se zvolí podle aktivního shader programu (pomocí useProgram).<br>
    /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
   

    auto& VertexPuller = *vertexpullerActive;
    auto& Program = *programActive;
    std::vector<OutVertex>VertexOUT;
    InVertex VertexIN;
    if (VertexPuller.indexing.type == IndexType::UINT32)
    {
        IndexType indexType = VertexPuller.indexing.type;
        std::vector<uint32_t>Buffer = reinterpret_cast<std::vector<uint32_t>&>(buffers.at(VertexPuller.indexing.currentBuffer));
        VertexOUT.reserve(Buffer.size());
        for (auto it = Buffer.begin(), end = Buffer.end(); it != end; it++)
        {
            IndexType indexType = VertexPuller.indexing.type;
            uint32_t vertexid = 0;
            VertexIN.gl_VertexID = uint32_t(*it);

            for (uint8_t i = 0; i < maxAttributes; i++)
            {
                if (VertexPuller.Headsettings[i].Enabled == true)
                {
                    switch (VertexPuller.Headsettings[i].type)
                    {

                    case AttributeType::FLOAT:
                        VertexIN.attributes[i].v1 = (float&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC2:
                        VertexIN.attributes[i].v2 = (glm::vec2&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC3:
                        VertexIN.attributes[i].v3 = (glm::vec3&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC4:
                        VertexIN.attributes[i].v4 = (glm::vec4&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    default:
                        break;
                    }
                }
            }
            auto& Vertex = VertexOUT.emplace_back();
            vertexid++;
            Program.VertexShader(Vertex, VertexIN, Program.Uniforms);
        }
    }
    else if (VertexPuller.indexing.type == IndexType::UINT16)
    {
        IndexType indexType = VertexPuller.indexing.type;
        auto& Bufferr = buffers.at(VertexPuller.indexing.currentBuffer);
        std::vector<uint16_t>Buffer = reinterpret_cast<std::vector<uint16_t>&>(Bufferr);
        VertexOUT.reserve(Buffer.size());
        for (auto it = Buffer.begin(), end = Buffer.end(); it != end; it++)
        {
            IndexType indexType = VertexPuller.indexing.type;
            uint32_t vertexid = 0;
            VertexIN.gl_VertexID = uint32_t(*it);

            for (uint8_t i = 0; i < maxAttributes; i++)
            {
                if (VertexPuller.Headsettings[i].Enabled == true)
                {
                    switch (VertexPuller.Headsettings[i].type)
                    {

                    case AttributeType::FLOAT:
                        VertexIN.attributes[i].v1 = (float&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC2:
                        VertexIN.attributes[i].v2 = (glm::vec2&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC3:
                        VertexIN.attributes[i].v3 = (glm::vec3&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC4:
                        VertexIN.attributes[i].v4 = (glm::vec4&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    default:
                        break;
                    }
                }
            }
            auto& Vertex = VertexOUT.emplace_back();
            vertexid++;
            Program.VertexShader(Vertex, VertexIN, Program.Uniforms);
        }
    }
    else if (VertexPuller.indexing.type == IndexType::UINT8)
    {
        IndexType indexType = VertexPuller.indexing.type;
        auto& Bufferr = buffers.at(VertexPuller.indexing.currentBuffer);
        std::vector<uint8_t>Buffer = reinterpret_cast<std::vector<uint8_t>&>(Bufferr);
        VertexOUT.reserve(Buffer.size());
        for (auto it = Buffer.begin(), end = Buffer.end(); it != end; it++)
        {
            IndexType indexType = VertexPuller.indexing.type;
            uint32_t vertexid = 0;
            VertexIN.gl_VertexID = uint32_t(*it);

            for (uint8_t i = 0; i < maxAttributes; i++)
            {
                if (VertexPuller.Headsettings[i].Enabled == true)
                {
                    switch (VertexPuller.Headsettings[i].type)
                    {

                    case AttributeType::FLOAT:
                        VertexIN.attributes[i].v1 = (float&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC2:
                        VertexIN.attributes[i].v2 = (glm::vec2&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC3:
                        VertexIN.attributes[i].v3 = (glm::vec3&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC4:
                        VertexIN.attributes[i].v4 = (glm::vec4&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    default:
                        break;
                    }
                }
            }
            auto& Vertex = VertexOUT.emplace_back();
            vertexid++;
            Program.VertexShader(Vertex, VertexIN, Program.Uniforms);
        }
    }
    else
    {
        VertexOUT.resize(nofVertices);
        uint32_t vertexid = 0;
        for (auto it = VertexOUT.begin(), end = VertexOUT.end(); it != end; ++it)
        {
            auto& Vertex = *it;

            VertexIN.gl_VertexID = vertexid;
            for (uint8_t i = 0; i < maxAttributes; i++)
            {
                if (VertexPuller.Headsettings[i].Enabled == true)
                {
                    switch (VertexPuller.Headsettings[i].type)
                    {

                    case AttributeType::FLOAT:
                        VertexIN.attributes[i].v1 = (float&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC2:
                        VertexIN.attributes[i].v2 = (glm::vec2&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC3:
                        VertexIN.attributes[i].v3 = (glm::vec3&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break;

                    case AttributeType::VEC4:
                        VertexIN.attributes[i].v4 = (glm::vec4&)buffers.at(VertexPuller.Headsettings[i].currentBuffer)[VertexIN.gl_VertexID * VertexPuller.Headsettings[i].stride + VertexPuller.Headsettings[i].offset];
                        break; 

                    default:
                        break;
                    }
                }
 
            }
            vertexid++;
            Program.VertexShader(Vertex, VertexIN, Program.Uniforms);
            auto a = Vertex.gl_Position;
        }


    }

 

    
        for (uint32_t i = 0; i < VertexOUT.size(); i += 3)
        {
            auto& v0 = VertexOUT[i];
            auto& v1 = VertexOUT[i + 1];
            auto& v2 = VertexOUT[i + 2];
            ClipTriangles(v0, v1, v2);
        }
}



void GPU::ClipTriangles(OutVertex& V0, OutVertex& V1, OutVertex& V2)
{
    
    auto& Program = *programActive;
    auto size = programActive->Attributes;


    uint8_t correct_V00X = 1, correct_V00Y = 1, correct_V00Z = 1, correct_V10X = 1, correct_V10Y = 1, correct_V10Z = 1, correct_V20X = 1, correct_V20Y = 1, correct_V20Z = 1;
    uint8_t correct_V01X = 1, correct_V01Y = 1, correct_V01Z = 1, correct_V11X = 1, correct_V11Y = 1, correct_V11Z = 1, correct_V21X = 1, correct_V21Y = 1, correct_V21Z = 1;
    bool IsV0OUT = 1, IsV1OUT = 1, IsV2OUT = 1;


    if (-V0.gl_Position.w <= V0.gl_Position.x)correct_V00X = 0;
    if (-V0.gl_Position.w <= V0.gl_Position.y)correct_V00Y = 0;
    if (-V0.gl_Position.w <= V0.gl_Position.z)correct_V00Z = 0;

    if (-V1.gl_Position.w <= V1.gl_Position.x)correct_V10X = 0;
    if (-V1.gl_Position.w <= V1.gl_Position.y)correct_V10Y = 0;
    if (-V1.gl_Position.w <= V1.gl_Position.z)correct_V00Z = 0;

    if (-V2.gl_Position.w <= V2.gl_Position.x)correct_V20X = 0;
    if (-V2.gl_Position.w <= V2.gl_Position.y)correct_V20Y = 0;
    if (-V2.gl_Position.w <= V2.gl_Position.z)correct_V20Z = 0;


    if (V0.gl_Position.w >= V0.gl_Position.x)correct_V01X = 0;
    if (V0.gl_Position.w >= V0.gl_Position.y)correct_V01Y = 0;
    if (V0.gl_Position.w >= V0.gl_Position.z)correct_V01Z = 0;

    if (V1.gl_Position.w >= V1.gl_Position.x)correct_V11X = 0;
    if (V1.gl_Position.w >= V1.gl_Position.y)correct_V11Y = 0;
    if (V1.gl_Position.w >= V1.gl_Position.z)correct_V01Z = 0;

    if (V2.gl_Position.w >= V2.gl_Position.x)correct_V21X = 0;
    if (V2.gl_Position.w >= V2.gl_Position.y)correct_V21Y = 0;
    if (V2.gl_Position.w >= V2.gl_Position.z)correct_V21Z = 0;
    /*
    //if triangle is out
    if ((correct_V01X & correct_V01Y & correct_V01Z && correct_V11X & correct_V11Y & correct_V11Z && correct_V21X & correct_V21Y & correct_V21Z) ==1)return;

    //if triangle is out
    if ((correct_V00X & correct_V00Y & correct_V00Z && correct_V10X & correct_V10Y & correct_V10Z && correct_V20X & correct_V20Y & correct_V20Z) == 1)return;
    /*
    if (-V0.gl_Position.w <= V0.gl_Position.x && V0.gl_Position.w >= V0.gl_Position.x)correct_0X = 1;
    if (-V0.gl_Position.w <= V0.gl_Position.y && V0.gl_Position.w >= V0.gl_Position.y)correct_0Y = 1;
    if (-V0.gl_Position.w <= V0.gl_Position.z && V0.gl_Position.w >= V0.gl_Position.z)correct_0Z = 1;

    if (-V1.gl_Position.w <= V1.gl_Position.x && V1.gl_Position.w >= V1.gl_Position.x)correct_1X = 1;
    if (-V1.gl_Position.w <= V1.gl_Position.y && V1.gl_Position.w >= V1.gl_Position.y)correct_1Y = 1;
    if (-V1.gl_Position.w <= V1.gl_Position.z && V1.gl_Position.w >= V1.gl_Position.z)correct_0Z = 1;

    if (-V2.gl_Position.w <= V2.gl_Position.x && V2.gl_Position.w >= V2.gl_Position.x)correct_2X = 1;
    if (-V2.gl_Position.w <= V2.gl_Position.y && V2.gl_Position.w >= V2.gl_Position.y)correct_2Y = 1;
    if (-V2.gl_Position.w <= V2.gl_Position.z && V2.gl_Position.w >= V2.gl_Position.z)correct_2Z = 1;
    
    //if triangle is correct [0000]
    if ((correct_V00X | correct_V00Y | correct_V00Z || correct_V10X | correct_V10Y | correct_V10Z || correct_V20X | correct_V20Y | correct_V20Z || correct_V01X | correct_V01Y | correct_V01Z || correct_V11X | correct_V11Y | correct_V11Z || correct_V21X | correct_V21Y | correct_V21Z) == 0)
        ;//ProcessTringle(V0,V1,V2);
    */

    //BIT MASK  [1001]  [0001]  [0101] mzymask  [0110]  [0010]  [1010]
    //          [1000]  [0000]  [0100]          [0100]  [0000]  [1000]
    //          [1010]  [0010]  [0110]          [0101]  [0001]  [1001]

    if (-V0.gl_Position.w <= V0.gl_Position.z)  IsV0OUT = 0;//Nearplane
    if (correct_V00X < correct_V01X && correct_V00Y < correct_V01Y);//[0101]    V0
    if (correct_V00X < correct_V01X && correct_V00Y == correct_V01Y);//[0100]
    if (correct_V00X < correct_V01X && correct_V00Y > correct_V01Y);//[0110]
    if (correct_V00X == correct_V01X && correct_V00Y > correct_V01Y);//[0010]
    if (correct_V00X > correct_V01X && correct_V00Y > correct_V01Y);//[1010]
    if (correct_V00X > correct_V01X&& correct_V00Y == correct_V01Y);//[1000]
    if (correct_V00X > correct_V01X&& correct_V00Y < correct_V01Y);//[1001]
    if (correct_V00X == correct_V01X && correct_V00Y < correct_V01Y);//[0001]
    if (correct_V00X == correct_V01X && correct_V00Y == correct_V01Y);//[0000]

    if (-V1.gl_Position.w <= V1.gl_Position.z)  IsV1OUT = 0;//Nearplane
    if (correct_V10X < correct_V11X && correct_V10Y < correct_V11Y);//[0101]    V1
    if (correct_V10X < correct_V11X && correct_V10Y == correct_V11Y);//[0100]
    if (correct_V10X < correct_V11X && correct_V10Y > correct_V11Y);//[0110]
    if (correct_V10X == correct_V11X && correct_V10Y > correct_V11Y);//[0010]
    if (correct_V10X > correct_V11X&& correct_V10Y > correct_V11Y);//[1010]
    if (correct_V10X > correct_V11X&& correct_V10Y == correct_V11Y);//[1000]
    if (correct_V10X > correct_V11X&& correct_V10Y < correct_V11Y);//[1001]
    if (correct_V10X == correct_V11X && correct_V10Y < correct_V11Y);//[0001]
    if (correct_V10X == correct_V11X && correct_V10Y == correct_V11Y);//[0000]

    if (-V2.gl_Position.w <= V2.gl_Position.z)  IsV2OUT = 0;//Nearplane
    if (correct_V20X < correct_V21X && correct_V20Y < correct_V21Y);//[0101]    V2
    if (correct_V20X < correct_V21X && correct_V20Y == correct_V21Y);//[0100]
    if (correct_V20X < correct_V21X && correct_V20Y > correct_V21Y);//[0110]
    if (correct_V20X == correct_V21X && correct_V20Y > correct_V21Y);//[0010]
    if (correct_V20X > correct_V21X&& correct_V20Y > correct_V21Y);//[1010]
    if (correct_V20X > correct_V21X&& correct_V20Y == correct_V21Y);//[1000]
    if (correct_V20X > correct_V21X&& correct_V20Y < correct_V21Y);//[1001]
    if (correct_V20X == correct_V21X && correct_V20Y < correct_V21Y);//[0001]
    if (correct_V20X == correct_V21X && correct_V20Y == correct_V21Y);//[0000]

    
    if (IsV0OUT && IsV1OUT && IsV2OUT)return;//Do nothing
    else if (!IsV0OUT && !IsV1OUT && !IsV2OUT)   ProcessTriangle(V0, V1, V2);
    else if (IsV0OUT && !IsV1OUT && !IsV2OUT)    Clipping(V0, V1, V2,1);//clip1 V0V1V2
    else if (IsV1OUT && !IsV0OUT && !IsV2OUT)    Clipping(V1, V0, V2,1);//clip1 V1V0V2
    else if (IsV2OUT && !IsV0OUT && !IsV1OUT)    Clipping(V2, V0, V1,1);//clip1 V2V0V1
    else if (IsV0OUT && IsV1OUT && !IsV2OUT)     Clipping(V0, V1, V2,0);//clip2 V0V1V2
    else if (IsV0OUT && IsV2OUT && !IsV1OUT)     Clipping(V0, V2, V1,0);//clip2 V0V2V1
    else if (IsV1OUT && IsV2OUT && !IsV0OUT)     Clipping(V1, V2, V0,0);//clip2 V1V2V0




}

void GPU::Clipping(OutVertex& V0, OutVertex& V1, OutVertex& V2, uint8_t verzion)
{/*
    if (verzion == 1)
    {
        //CLIP1
        float paramTa = (-V0.gl_Position.w - V0.gl_Position.z) / (V1.gl_Position.w - V0.gl_Position.w + V1.gl_Position.z - V0.gl_Position.z);
        float paramTb = (-V0.gl_Position.w - V0.gl_Position.z) / (V2.gl_Position.w - V0.gl_Position.w + V2.gl_Position.w - V0.gl_Position.z);
        auto va = V0;
        auto vb = V0;
        for (uint32_t i = 0; i < maxAttributes; i++)
            va.attributes[i].v4 = V0.attributes[i].v4 + paramTa * (V1.attributes[i].v4 - V0.attributes[i].v4);

        for (uint32_t i = 0; i < maxAttributes; i++)
            vb.attributes[i].v4 = V0.attributes[i].v4 + paramTb * (V2.attributes[i].v4 - V0.attributes[i].v4);
        //Process Triangle(va,V1,V2)
        //Process Triangle(vb,va,V2)
        ProcessTriangle(va, V1, V2);
        ProcessTriangle(vb, va, V2);
    }
    else
    {
        //CLIP2
        float paramTc = (-V0.gl_Position.w - V0.gl_Position.z) / (V2.gl_Position.w - V0.gl_Position.w + V2.gl_Position.z - V0.gl_Position.z);
        float paramTd = (-V1.gl_Position.w - V1.gl_Position.z) / (V2.gl_Position.w - V1.gl_Position.w + V2.gl_Position.w - V1.gl_Position.z);

        for (uint32_t i = 0; i < maxAttributes; i++)
            V0.attributes[i].v4 = V0.attributes[i].v4 + paramTc * (V2.attributes[i].v4 - V0.attributes[i].v4);

        for (uint32_t i = 0; i < maxAttributes; i++)
            V1.attributes[i].v4 = V1.attributes[i].v4 + paramTd * (V2.attributes[i].v4 - V1.attributes[i].v4);
        ProcessTriangle(V0, V1, V2);
    }
    */
}


void GPU::ProcessTriangle(OutVertex& V0, OutVertex& V1, OutVertex& V2)
{
    //NDC
    
    V0.gl_Position.x = V0.gl_Position.x / V0.gl_Position.w;
    V0.gl_Position.y = V0.gl_Position.y / V0.gl_Position.w;
    V0.gl_Position.z = V0.gl_Position.z / V0.gl_Position.w;

    V1.gl_Position.x = V1.gl_Position.x / V1.gl_Position.w;
    V1.gl_Position.y = V1.gl_Position.y / V1.gl_Position.w;
    V1.gl_Position.z = V1.gl_Position.z / V1.gl_Position.w;

    V2.gl_Position.x = V2.gl_Position.x / V2.gl_Position.w;
    V2.gl_Position.y = V2.gl_Position.y / V2.gl_Position.w;
    V2.gl_Position.z = V2.gl_Position.z / V2.gl_Position.w;

    //NDC to Screen-Space
    
    V0.gl_Position.x = V0.gl_Position.x * Width / 2 + Width / 2;
    V0.gl_Position.y = V0.gl_Position.y * Height / 2 + Height / 2;
    V0.gl_Position.z = V0.gl_Position.z;

    V1.gl_Position.x = V1.gl_Position.x * Width / 2 + Width / 2;
    V1.gl_Position.y = V1.gl_Position.y * Height / 2 + Height / 2;
    V1.gl_Position.z = V1.gl_Position.z;

    V2.gl_Position.x = V2.gl_Position.x * Width / 2 + Width / 2;
    V2.gl_Position.y = V2.gl_Position.y * Height / 2 + Height / 2;
    V2.gl_Position.z = V2.gl_Position.z;



    //Rasterizace
    //Pined algorythm

    auto delt_X01 = V0.gl_Position.x - V1.gl_Position.x;
    auto delt_Y01 = V0.gl_Position.y - V1.gl_Position.y;

    auto delt_X12 = V1.gl_Position.x - V2.gl_Position.x;
    auto delt_Y12 = V1.gl_Position.y - V2.gl_Position.y;

    auto delt_X20 = V2.gl_Position.x - V0.gl_Position.x;
    auto delt_Y20 = V2.gl_Position.y - V0.gl_Position.y;

    InFragment FragmentIN;
    OutFragment FragmentOUT;
    auto& Program = *programActive;

    
    for (uint32_t i = 0; i < Framebuffer.size(); i++)
    {
        uint32_t X = i / Width+1;
        uint32_t Y = i % Height+1;
        
        
        auto E01 = (X - V0.gl_Position.x) * delt_Y01 - (Y - V0.gl_Position.y) * delt_X01;
        auto E12 = (X - V1.gl_Position.x) * delt_Y12 - (Y - V1.gl_Position.y) * delt_X12;
        auto E20 = (X - V2.gl_Position.x) * delt_Y20 - (Y - V2.gl_Position.y) * delt_X20;

        /*
        if ((V1.gl_Position.x - V0.gl_Position.x) * (V2.gl_Position.y - V0.gl_Position.y) - (V1.gl_Position.y - V0.gl_Position.y) * (V2.gl_Position.x - V0.gl_Position.x))
        {
            auto temp = V1;
            V1 = V2;
            V2 = temp;
        }
        */

        if (E01 >= 0 && E12 >= 0 && E20 >= 0)
        {
            FragmentIN.gl_FragCoord.x = X - 0.5;// + 0.5f
            FragmentIN.gl_FragCoord.y = Y - 0.5;
            ///*

            /// BARYCENTRIC COORDINATIONS


            



            auto lmb0 = (E12 / 2) / ((E12 / 2) + (E20 / 2) + (E01 / 2));
            auto lmb1 = (E20 / 2) / ((E12 / 2) + (E20 / 2) + (E01 / 2));
            auto lmb2 = (E01 / 2) / ((E12 / 2) + (E20 / 2) + (E01 / 2));

            auto lmb = lmb0 + lmb1 + lmb2;   //  lmb == 1






            for (uint32_t j = 0; j < maxAttributes; j++)
            {
                
                FragmentIN.attributes[j].v1 = ((lmb0 * V0.attributes[j].v1 / V0.gl_Position.w) + (lmb1 * V1.attributes[j].v1 / V1.gl_Position.w) + (lmb2 * V2.attributes[j].v1 / V2.gl_Position.w)) / ((lmb0 / V0.gl_Position.w) + (lmb1 / V1.gl_Position.w) + (lmb2 / V2.gl_Position.w));
                FragmentIN.attributes[j].v2 = ((lmb0 * V0.attributes[j].v2 / V0.gl_Position.w) + (lmb1 * V1.attributes[j].v2 / V1.gl_Position.w) + (lmb2 * V2.attributes[j].v2 / V2.gl_Position.w)) / ((lmb0 / V0.gl_Position.w) + (lmb1 / V1.gl_Position.w) + (lmb2 / V2.gl_Position.w));
                FragmentIN.attributes[j].v3 = ((lmb0 * V0.attributes[j].v3 / V0.gl_Position.w) + (lmb1 * V1.attributes[j].v3 / V1.gl_Position.w) + (lmb2 * V2.attributes[j].v3 / V2.gl_Position.w)) / ((lmb0 / V0.gl_Position.w) + (lmb1 / V1.gl_Position.w) + (lmb2 / V2.gl_Position.w));
                FragmentIN.attributes[j].v4 = ((lmb0 * V0.attributes[j].v4 / V0.gl_Position.w) + (lmb1 * V1.attributes[j].v4 / V1.gl_Position.w) + (lmb2 * V2.attributes[j].v4 / V2.gl_Position.w)) / ((lmb0 / V0.gl_Position.w) + (lmb1 / V1.gl_Position.w) + (lmb2 / V2.gl_Position.w));
            }

            
            //*/


            Program.FragmentShader(FragmentOUT, FragmentIN, Program.Uniforms);

            if (FragmentIN.gl_FragCoord.z < depth[Y * Width + X])
            {/*
                Framebuffer[X + Y * Width].R = 255;
                Framebuffer[X + Y * Width].G = 255;
                Framebuffer[X + Y * Width].B = 255;
                Framebuffer[X + Y * Width].A = 255;
                depth[X + Y * Width] = FragmentIN.gl_FragCoord.z;*/
                
                Framebuffer[X + Y * Width].R = FragmentOUT.gl_FragColor.r*255;
                Framebuffer[X + Y * Width].G = FragmentOUT.gl_FragColor.g*255;
                Framebuffer[X + Y * Width].B = FragmentOUT.gl_FragColor.b*255;
                Framebuffer[X + Y * Width].A = FragmentOUT.gl_FragColor.a*255;
                depth[X + Y * Width] = FragmentIN.gl_FragCoord.z;
            }



            

        }
        else {}
        
       

    }
   
    
}









/// @}











/*
OutFragment o;
InFragment POP;

auto& Program = *programActive;
for (int i = 0; i < 4900; i++)
    Program.FragmentShader(o, POP, Program.Uniforms);
    */