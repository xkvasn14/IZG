/*!
 * @file
 * @brief This file contains class that represents graphic card.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#pragma once

#include <student/fwd.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <array>



/**
 * @brief This class represent software GPU
 */
class GPU{
  public:
    GPU();
    virtual ~GPU();

    //buffer object commands
    BufferID  createBuffer           (uint64_t size);
    void      deleteBuffer           (BufferID buffer);
    void      setBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void const* data);
    void      getBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void      * data);
    bool      isBuffer               (BufferID buffer);

    //vertex array object commands (vertex puller)
    ObjectID  createVertexPuller     ();
    void      deleteVertexPuller     (VertexPullerID vao);
    void      setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer);
    void      setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer);
    void      enableVertexPullerHead (VertexPullerID vao,uint32_t head);
    void      disableVertexPullerHead(VertexPullerID vao,uint32_t head);
    void      bindVertexPuller       (VertexPullerID vao);
    void      unbindVertexPuller     ();
    bool      isVertexPuller         (VertexPullerID vao);

    //program object commands
    ProgramID createProgram          ();
    void      deleteProgram          (ProgramID prg);
    void      attachShaders          (ProgramID prg,VertexShader vs,FragmentShader fs);
    void      setVS2FSType           (ProgramID prg,uint32_t attrib,AttributeType type);
    void      useProgram             (ProgramID prg);
    bool      isProgram              (ProgramID prg);
    void      programUniform1f       (ProgramID prg,uint32_t uniformId,float     const&d);
    void      programUniform2f       (ProgramID prg,uint32_t uniformId,glm::vec2 const&d);
    void      programUniform3f       (ProgramID prg,uint32_t uniformId,glm::vec3 const&d);
    void      programUniform4f       (ProgramID prg,uint32_t uniformId,glm::vec4 const&d);
    void      programUniformMatrix4f (ProgramID prg,uint32_t uniformId,glm::mat4 const&d);

    //framebuffer functions
    void      createFramebuffer      (uint32_t width,uint32_t height);
    void      deleteFramebuffer      ();
    void      resizeFramebuffer      (uint32_t width,uint32_t height);
    uint8_t*  getFramebufferColor    ();
    float*    getFramebufferDepth    ();
    uint32_t  getFramebufferWidth    ();
    uint32_t  getFramebufferHeight   ();

    //execution commands
    void      clear                  (float r,float g,float b,float a);
    void      drawTriangles          (uint32_t  nofVertices);

    void ClipTriangles(OutVertex& V0, OutVertex& V1, OutVertex& V2);
    void Clipping(OutVertex& V0, OutVertex& V1, OutVertex& V2, uint8_t verzion);
    void ProcessTriangle(OutVertex& V0, OutVertex& V1, OutVertex& V2);
    bool DepthTest(uint32_t width, uint32_t height, float z);

    struct HeadSettings
    {
        BufferID currentBuffer;
        uint64_t offset;
        uint64_t stride;
        AttributeType type;
        bool Enabled = false;
    };


    struct Indexing
    {
        BufferID currentBuffer;
        IndexType type;
    };

    struct VertexPuller
    {
        Indexing indexing;
        HeadSettings Headsettings[maxAttributes];
        BufferID activeBuffer = emptyID;
    };


    struct ProgramSettings
    {
        VertexShader VertexShader;
        FragmentShader FragmentShader;
        Uniforms Uniforms;
        AttributeType Attributes [maxAttributes];
    };

    struct ColorBuffer
    {
        uint8_t R;
        uint8_t G;
        uint8_t B;
        uint8_t A;
    };


    //BUFFERS VERTEXPULLERS PROGRAMS
    std::unordered_map<BufferID, std::vector<uint8_t>> buffers;
    //std::unordered_map<VertexPullerID, std::vector<VertexPuller>> vertexPullers;                               
    std::unordered_map<VertexPullerID, std::unique_ptr<VertexPuller>> vertexPullers;
    std::unordered_map<ProgramID, std::unique_ptr<ProgramSettings>> programs;

    //FRAMEBUFFER
    std::vector<float> depth;
    std::vector<ColorBuffer> Framebuffer;
    uint32_t Width;
    uint32_t Height;



    
    //ACTIVES
    VertexPuller* vertexpullerActive;
    ProgramSettings* programActive;









};

