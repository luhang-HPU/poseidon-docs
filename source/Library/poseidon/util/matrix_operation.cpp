#include "matrix_operation.h"

using namespace std;
namespace matrix_operations
{
//vec_product(m*1) = matrix(m*n) * vec(n*1)



//vec_sum = vec_a + vec_b
    void add(vector<complex<double> > vec_a, vector<complex<double> > vec_b, vector<complex<double> > &vec_sum)
    {
        vector<complex<double> > c;
        for(int i=0; i<vec_a.size(); ++i)
            c.push_back(vec_a[i] + vec_b[i]);
        vec_sum.swap(c);
        return;
    }

    void multiply(vector<complex<double> > vec_a, vector<complex<double> > vec_b, vector<complex<double> > &vec_sum)
    {
        vector<complex<double> > c;
        for(int i=0; i<vec_a.size(); ++i)
            c.push_back(vec_a[i] * vec_b[i]);
        vec_sum.swap(c);
        return;
    }

//matrix_sum = matrix_a + matrix_b
    void matrix_add(vector<vector<complex<double> > > matrix_a, vector<vector<complex<double> > > matrix_b, vector<vector<complex<double> > > &matrix_sum)
    {
        vector<vector<complex<double> > > matrix_c;
        for(int i=0; i<matrix_a.size(); ++i)
        {
            vector<complex<double> > c;
            for(int j=0; j<matrix_a[0].size(); ++j)
            {
                c.push_back(matrix_a[i][j] + matrix_b[i][j]);
            }
            matrix_c.push_back(c);
        }
        matrix_sum.swap(matrix_c);
        return;
    }

/*
vector<vector<complex<double> > > subtract(vector<vector<complex<double> > > matrix_a, vector<vector<complex<double> > > matrix_b)
{
    vector<vector<complex<double> > > matrix_c;
    for(int i=0; i<matrix_a.size(); ++i)
    {
        vector<complex<double> > c;
        for(int j=0; j<matrix_a[0].size(); ++j)
        {
            c.push_back(matrix_a[i][j] - matrix_b[i][j]);
        }
        matrix_c.push_back(c);
    }
    return matrix_c;
}
*/

//matrix_product = matrix_a * n
    void scalar_multiply(vector<vector<complex<double> > > matrix_a, double n, vector<vector<complex<double> > > &matrix_product)
    {
        vector<vector<complex<double> > > matrix_c;
        for(int i=0; i<matrix_a.size(); ++i)
        {
            vector<complex<double> > c;
            for(int j=0; j<matrix_a[0].size(); ++j)
            {
                c.push_back(matrix_a[i][j] *n);
            }
            matrix_c.push_back(c);
        }
        matrix_product.swap(matrix_c);
        return;
    }

//matrix_product = matrix_a * matrix_b
    void multiply(vector<vector<complex<double> > > matrix_a, vector<vector<complex<double> > > matrix_b, vector<vector<complex<double> > > &matrix_product)
    {
        vector<vector<complex<double> > > matrix_c;
        for(int i=0; i<matrix_a.size(); ++i)
        {
            vector<complex<double> > c;
            for(int j=0; j<matrix_b[0].size(); ++j)
            {
                complex<double> tmp(0, 0);
                for(int k=0; k<matrix_a[0].size(); ++k)
                {
                    tmp += matrix_a[i][k] * matrix_b[k][j];
                }
                c.push_back(tmp);
            }
            matrix_c.push_back(c);
        }
        matrix_product.swap(matrix_c);
        return;
    }

//vec_diag = matrix_a.diagonal(diag_index)


//vec_rotate = vec.rotate(rotation)


//matrix_conj = conjugate of matrix
    void conjugate_matrix(vector<vector<complex<double> > > matrix, vector<vector<complex<double> > > &matrix_conj)
    {
        vector<vector<complex<double> > > conj_matrix;
        for(int i=0; i<matrix.size(); ++i)
        {
            vector<complex<double> > tmp;
            for(int j=0; j<matrix[0].size(); ++j)
            {
                tmp.push_back(conj(matrix[i][j]));
            }
            conj_matrix.push_back(tmp);
        }
        matrix_conj.swap(conj_matrix);
        return;
    }

//matrix_trans = transpose of matrix


    void PrintVec(vector<vector<complex<double> > >& A)
    {
        for (int i = 0; i < A.size(); i++)
        {
            for (int j = 0; j < A[i].size(); j++)
            {
                cout << A[i][j] << " ";
            }
            cout << endl;
        }
        return;
    }

}
/*
int main()
{
    vector<vector<complex<double> > >matrix_a;
    vector<complex<double> > a;
    a.push_back(complex<double>(12, 1));
    a.push_back(complex<double>(10, 2));
    matrix_a.push_back(a);
    a.clear();
    a.push_back(complex<double>(3, 3));
    a.push_back(complex<double>(9, 4));
    matrix_a.push_back(a);

    cout << "A:";
    PrintVec(matrix_a);
    cout << endl;

    vector<vector<complex<double> > >matrix_b;
    vector<complex<double> > b;
    b.push_back(complex<double>(3, 5));
    b.push_back(complex<double>(4, 6));
    matrix_b.push_back(b);
    b.clear();
    b.push_back(complex<double>(7, 7));
    b.push_back(complex<double>(4, 8));
    matrix_b.push_back(b);

    cout << "B:";
    PrintVec(matrix_b);
    cout << endl;
    
    {
        vector<vector<complex<double> > > matrix_c;
        matrix_add(matrix_a, matrix_b, matrix_c);
        cout << "A+B:";
        PrintVec(matrix_c);
        cout << endl;
    }
    
    {
        int n = 2;
        vector<vector<complex<double> > > matrix_c;
        scalar_multiply(matrix_a, n, matrix_c);
        cout << "A*2:";
        PrintVec(matrix_c);
        cout << endl;
    }
    
    {
        vector<vector<complex<double> > > matrix_c;
        multiply(matrix_a, matrix_b, matrix_c);
        cout << "A*B:";
        PrintVec(matrix_c);
        cout << endl;
    }
    
    {
        size_t diag_index = 0;
        vector<complex<double> > vec_a;
        diagonal(matrix_a, diag_index, vec_a);
        cout << "diag of A:";
        for (int i=0; i<vec_a.size(); ++i)
            cout << vec_a[i] << " ";
        cout << endl << endl;
        vector<complex<double> > vec_b;
        diagonal(matrix_b, diag_index, vec_b);
        vec_a.push_back(vec_b[0]);
        vec_a.push_back(vec_b[1]);
        cout << "vec:";
        for (int i=0; i<vec_a.size(); ++i)
            cout << vec_a[i] << " ";
        cout << endl;

        size_t rotation = 1;
        vector<complex<double> > vec;
        rotate(vec_a, rotation, vec);
        cout << "left rotate 1 of vec:";
        for (int i=0; i<vec.size(); ++i)
            cout << vec[i] << " ";
        cout << endl;

        vector<complex<double> > vec_c;
        add(vec, vec_a, vec_c);
        cout << "sum of above:";
        for (int i=0; i<vec_c.size(); ++i)
            cout << vec_c[i] << " ";
        cout << endl;

        vec_a.resize(2);
        vector<complex<double> > vec_prod;
        vec_a[0] = complex<double>(3, 5);
        vec_a[1] = complex<double>(7, 7);
        matrix_vector_multiply(matrix_a, vec_a, vec_prod);
        cout << "A*B[][0]:";
        for (int i=0; i<vec_prod.size(); ++i)
            cout << vec_prod[i] << " ";
        cout << endl;
    }

    {
        vector<vector<complex<double> > > matrix_c;
        conjugate_matrix(matrix_a, matrix_c);
        cout << "_\nA:";
        PrintVec(matrix_c);
        cout << endl;
    }

    {
        vector<vector<complex<double> > > matrix_c;
        transpose_matrix(matrix_a, matrix_c);
        cout << "A^T:";
        PrintVec(matrix_c);
        cout << endl;
    }

    return 0;
}
*/