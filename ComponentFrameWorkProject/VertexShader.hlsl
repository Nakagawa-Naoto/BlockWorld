
cbuffer World : register(b0)
{
    Matrix world;
};
cbuffer View : register(b1)
{
    Matrix view;
};
cbuffer Projection : register(b2)
{
    Matrix projection;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
      
    matrix wvp;
    wvp = mul(world, view);
    wvp = mul(wvp, projection);
    
    pos= mul(pos, wvp);
    
	return pos;
}