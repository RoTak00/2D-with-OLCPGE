struct vec2d
{
	float x = 0;
	float y = 0;
    float t = 1;

    vec2d(float _x, float _y)
    {
        x = _x; y = _y; t = 1;
    }

    vec2d()
    {
        x = 0; y = 0; t = 1;
    }
};

struct line
{
	vec2d p[2];
};

struct mat3x3
{
	float m[3][3];
};

struct object
{
	std::vector<line> lines;

    void loadFromFile(std::string fileName)
    {
        std::ifstream fin(fileName);

        while(!fin.eof())
        {
            line t;
            fin >> t.p[0].x;
            fin >> t.p[0].y;

            fin >> t.p[1].x;
            fin >> t.p[1].y;

            lines.push_back(t);             
        }
    }
};

vec2d Vec_Add(vec2d &v1, vec2d &v2)
{
    return {v1.x + v2.x, v1.y + v2.y};
}

vec2d Vec_Sub(vec2d &v1, vec2d &v2)
{
    return {v1.x - v2.x, v1.y - v2.y};
}

vec2d Vec_Mul(vec2d &v1, float k)
{
    return {v1.x * k, v1.y * k};
}
vec2d Vec_MultiplyMatrix(vec2d &in, mat3x3& m)
{
    vec2d r;
    r.x = m.m[0][0] * in.x + m.m[0][1] * in.y + m.m[0][2] * in.t;
    r.y = m.m[1][0] * in.x + m.m[1][1] * in.y + m.m[1][2] * in.t;
    r.t = m.m[2][0] * in.x + m.m[2][1] * in.y + m.m[2][2] * in.t;
    return r;
}

void Vec_Print(const char* name, vec2d &in)
{
    printf(" %s: {%3.3f, %3.3f}\n", name, in.x, in.y);
}

mat3x3 Mat_MultiplyMatrix(mat3x3 &m1, mat3x3 &m2)
{
    mat3x3 matrix;
    for(int c = 0; c < 3; c++)
        for(int r = 0; r < 3; r++)
            matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
    return matrix;
}

mat3x3 Mat_MakeTranslation(float dx, float dy)
{
    mat3x3 matrix;
    matrix.m[0][0] = 1; matrix.m[0][1] = 0; matrix.m[0][2] = dx;
    matrix.m[1][0] = 0; matrix.m[1][1] = 1; matrix.m[1][2] = dy;
    matrix.m[2][0] = 0; matrix.m[2][1] = 0; matrix.m[2][2] = 1;
    return matrix;
}

mat3x3 Mat_MakeScale(float k)
{
    mat3x3 matZoom;
    matZoom.m[0][0] = k; matZoom.m[1][0] = 0.0f; matZoom.m[2][0] = 0.0f;
		matZoom.m[0][1] = 0.0f; matZoom.m[1][1] = k; matZoom.m[2][1] = 0.0f;
		matZoom.m[0][2] = 0.0f; matZoom.m[1][2] = 0.0f; matZoom.m[2][2] = 1.0f;
    return matZoom;
}

mat3x3 Mat_MakeIdentity()
{
    mat3x3 mat;
    mat.m[0][0] = 1.0f; mat.m[1][0] = 0.0f; mat.m[2][0] = 0.0f;
		mat.m[0][1] = 0.0f; mat.m[1][1] = 1.0f; mat.m[2][1] = 0.0f;
		mat.m[0][2] = 0.0f; mat.m[1][2] = 0.0f; mat.m[2][2] = 1.0f;
    return mat;
}

mat3x3 Mat_FlipY()
{
    mat3x3 mat;
    mat.m[0][0] = 1.0f; mat.m[1][0] = 0.0f; mat.m[2][0] = 0.0f;
		mat.m[0][1] = 0.0f; mat.m[1][1] = -1.0f; mat.m[2][1] = 0.0f;
		mat.m[0][2] = 0.0f; mat.m[1][2] = 0.0f; mat.m[2][2] = 1.0f;
        return mat;
}



mat3x3 Mat_MakeRotation(float t)
{
    
    mat3x3 matRot;
    matRot.m[0][0] = cosf(t);
    matRot.m[0][1] = sinf(t);
    matRot.m[0][2] = 0.0f;
    matRot.m[1][0] = -sinf(t);
    matRot.m[1][1] = cosf(t);
    matRot.m[1][2] = 0.0f;
    matRot.m[2][0] = 0.0f;
    matRot.m[2][1] = 0.0f;
    matRot.m[2][2] = 1.0f;
    return matRot;
    
}