# COMP3015 CW2 Huw Jenkins
This is the codebase for my Coursework 2 Street Scene based on early PS1 games such as Resident Evil and Silent Hill.
## Which version Visual Studio and Operating System you used to test/write the code locally?
Visual Studio 2022, Microsoft Windows 11.
## How does it work? How does your code fit together and how should a programmer navigate it?
Sobel Edge detection is used. Implemented via using a Framebuffer object to allow rendering to things aside from the screen. Instead we render color and depth to a renderTexture and DepthTexture variable. These are passed into a post process fragment shader called rendertex.frag. Inside, a pair of convolution kernels are used to detect changes in depth sampled from the depthTexture. One detects horizontal changes, the other vertical. at a given fragment, if the red component of the Sobel filter output is less than 0.01, set the color to 1, otherwise set it to 0, and then multiply the current color by this result.

PBR Lighting is used. Implemented in the basic_uniform.frag fragment shader. We have the usual functions: three for calculating cook-torrance brdf, and a final one, microFacet which calculates the actual lighting and applies it to FragColor. We also have alpha clipping and fog applied to the final FragColour.
## Anything else which will help us to understand how your prototype works.
WASD to move, Mouse to look. There's no collision so you can go and see the backfaces of the shops if you'd like.

The .exe version can run without any help-- it's in STANDALONE BUILD x64/Release.
## A Link to the unlisted YouTube Video 
https://www.youtube.com/watch?v=cwShkdjkTw0
