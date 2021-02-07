# quark-shaders

A repository of GLSL #150 pixel shaders for use with the higan emulation
package.

The copyright of these shaders belong to their respective authors and they
are licensed under the General Public License v2 unless otherwise stated.

## ----- Spec Description -----

### Basic concept:


All files related to a shader (manifest, vertex shaders, geometry shaders, fragment shaders, texture images) go into one folder.
If you really need subfolders that's fine, but never reference the parent folder with ../ in a path. Each shader must be self-contained.

Shaders can be OpenGL 3.2+, GLSL 1.5+ only.

All textures are their actual size. It's not the '90s, so we don't need power-of-two textures anymore.

### manifest.bml:

Every shader must have a manifest.bml file. This is the file that lists all of the shader passes, textures, etc.

BML is basically a lighter weight version of XML. It's very similar to YAML, but a bit simpler still. Really, if you can't look at it and see how it works, you probably shouldn't be writing shader programs :/

Each shader pass is a "program". Each program can have one vertex shader, one geometry shader, one fragment shader, and unlimited textures. Each texture is a "pixmap", and should be stored in PNG format (any PNG color format is fine, including alpha channel info.) They run in the order they appear inside the manifest.

Lots of parameters are available.
```program
  width: 200%
  height: 200%
  filter: nearest
  wrap: border
  modulo: 800
  format: rgba8
  vertex: zoom.vs
  geometry: zoom.gs
  fragment: zoom.fs
  pixmap: texture.png
    filter: nearest
    wrap: repeat
    format: rgba16f
```	
First, we have width and height. This can be in relative values (eg 200%), or in absolute values (eg 512). This sets the output size for this pass. Relative width is relative to the previous pass. If there is no previous pass, then it's relative to the source input from the emulator (eg 240x160 for Game Boy Advance.) If you omit width and/or height, they will be read in as 0, which has a special meaning of being equal to the full output resolution.

The filter is what controls sampling when you call texture() within shaders. nearest is pixellated, and linear is blurry.

The modulo determines when the phase uniform wraps inside of shaders. The phase uniform is a counter that increments every frame rendered, and wraps once it reaches the modulo value. Thus, phase = (frames % modulo). So a value of 3 would have 0 on the first frame, 1 on the second frame, 2 on the third frame, 0 on the fourth frame, etc. Which would be a good value for simulating NES NTSC effects, for instance.

Every program only gets one phase counter. So if you need two or more counters, then use a modulo that is the LCD of all numbers, and then do your own division inside the shader. The only reason I expose modulo at all is because otherwise wrapping could eventually hit an error if you ran the emulator for 2.25 years straight. Which I imagine to be a very poignant concern :P

You can also control the format of the program's texture surface. Supported values are rgba8, rgb10a2, rgba12, rgba16, rgba16f, rgba32f. It's a tradeoff between texture size and quality. You'd only really want to use higher than the default rgba8 if you wanted to avoid subtle rounding errors between passes, or if you wanted to support eg 30-bit monitors better.

Then there's wrap, which can be border, edge or repeat. This decides what happens when you reference an area outside of the input texture. Important for filters like curvature and edge.

Next up, you specify your vertex, geometry and fragment shaders. If you don't provide one, it substitutes in a generic one that has no effect. So you can have a vertex-only shader for overscan simulation, or a fragment-only shader for bloom simulation. Or both.

### Vertex shaders
Vertex shaders control the points of the triangle strip, so it'll always be called four times for the four corners of our screen rectangle. We can use these to move the points that the screen is drawn at.

### Geometry shaders 
Geometry shaders control the output of triangles. It'll get called once for the first triangle, and a second time for the second triangle. Like in vertex shaders, we can also move the points where the screen is drawn. But now we can also output additional triangles. I can't really imagine where that would be useful, but maybe we'll be surprised one day by a neat effect. At the very least, it's probably a bit easier than vertex shaders for vertex-style effects.

### Fragment shaders
Fragment shaders control the color of each pixel output from the geometry stage. So this is where 99% of the useful stuff happens with 2D emulators like higan.

Next you can import lookup textures with pixmap. Give it a filename to load. Now you can also set the filter and wrap mode on the lookup texture. Note that you have to load a pixmap into each program you want to use it in. I don't share these, because some video cards have small limits to the number of textures bound at one time.

All values are optional. Defaults if a value is omitted:	
```program
  width: 0
  height: 0
  filter: linear
  modulo: 300
  format: rgba8
  wrap: border
  pixmap: [filename is required]
    filter: linear
    wrap: border
```
There is a history block that allows you to setup references to previous input frames, for effects like motion blur.
```history
  frames: ... [default: 0]
  format: ... [default: rgba8]
  filter: ... [default: linear]
  wrap: ... [default: border]
```
Access these via the uniforms: historyLength, history[], historySize[]. Because you're limited to input frames, rather than output frames, this constitutes FIR filtering, as opposed to IIR filtering.

Finally, there's one last pass. Just titled "output" instead of "program", this lets us control getting the final shader pass' output onto the screen. We can apply width, height and filter to this pass. So you could do a nice 200% upscale with HQ2x, and then let the output pass scale it to fill the display with linear interpolation.

### Shader uniforms:

The following uniforms are exposed to shaders:

modelView(4fv), projection(4fv), modelViewProjection(4fv), vertex(4fv), position(4fv), texCoord(2f) - the standard uniforms for mapping screen coordinates to texture coordinates. The gl_* versions were nearly all deprecated with OpenGL 3.2, so we have to expose them ourselves now.

```fragColor(4f){rgba} - write this in the fragment shader to determine final output color for each pixel

phase(1i) - see above, it's a counter that wraps with the manifest's modulo.

sourceLength(1i) - number of textures in the source array

pixmapLength(1i) - number of lookup textures bound to this program

targetSize(4f){width, height, 1.0 / width, 1.0 / height} - output size for the current pass, and its normalized friends

outputSize(4f) - final output size after all shaders have run

source(1i)[] - array of all previous pass output textures. source[0] is the previous pass, source[1] is the second to last pass, source[2] is the third to last pass, etc. Check sourceLength to determine how many passes are available. The ID can be used in texture() to read from it

sourceSize(4f)[] - sizes of each previous pass

pixmap(1i)[] - array of all lookup texture IDs. Use pixmapLength to determine how many there are, or just use common sense since you are specifying these in the manifest anyway.

pixmapSize(4f)[] - size of all lookup texture IDs
```
### Vertex -> Geometry -> Fragment:
Your vertex shader needs to output a class type to pass information to the geometry and fragment shaders. This is needed to get the array syntax used by geometry shaders. And it's also important because below are the default shader files if one is not specified in your manifest. So it's best to stick to this format.
```static string OpenGLVertexShader = R"(
  #version 150

  in vec4 position;
  in vec2 texCoord;

  out Vertex {
    vec2 texCoord;
  } vertexOut;

  void main() {
    gl_Position = position;
    vertexOut.texCoord = texCoord;
  }
)";

static string OpenGLGeometryShader = R"(
  #version 150

  layout(triangles) in;
  layout(triangle_strip, max_vertices = 3) out;

  in Vertex {
    vec2 texCoord;
  } vertexIn[];

  out Vertex {
    vec2 texCoord;
  };

  void main() {
    for(int i = 0; i < gl_in.length(); i++) {
      gl_Position = gl_in[i].gl_Position;
      texCoord = vertexIn[i].texCoord;
      EmitVertex();
    }
    EndPrimitive();
  }
)";

static string OpenGLFragmentShader = R"(
  #version 150

  uniform sampler2D source[];

  in Vertex {
    vec2 texCoord;
  };

  out vec4 fragColor;

  void main() {
    fragColor = texture(source[0], texCoord);
  }
)";
```
## ----- End Spec Description -----
