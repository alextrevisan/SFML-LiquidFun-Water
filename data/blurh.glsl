//     Description : 2D blur based on http://polycode.org/tutorial/screen_shaders
//     Author : Alexandro Trevisan
//     Lastmod : 20140521
//     License : Distributed under the MIT License
//               Based on http://polycode.org/tutorial/screen_shaders

uniform sampler2D screenColorBuffer;
const float blurSize = 1.0/1024.0;
 
void main(void)
{
   vec2 vTexCoord = gl_TexCoord[0].st;
   vec4 sum = vec4(0.0);
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x - 4.0*blurSize, vTexCoord.y)) * 0.05;
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x - 3.0*blurSize, vTexCoord.y)) * 0.09;
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x - 2.0*blurSize, vTexCoord.y)) * 0.12;
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x - blurSize, vTexCoord.y)) * 0.15;
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x, vTexCoord.y)) * 0.16;
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x + blurSize, vTexCoord.y)) * 0.15;
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x + 2.0*blurSize, vTexCoord.y)) * 0.12;
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x + 3.0*blurSize, vTexCoord.y)) * 0.09;
   sum += texture2D(screenColorBuffer, vec2(vTexCoord.x + 4.0*blurSize, vTexCoord.y)) * 0.05;
   gl_FragColor = sum;
}
