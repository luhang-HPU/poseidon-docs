#ifndef MATRIX_OPERATION_H
#define MATRIX_OPERATION_H

#include<iostream>
#include<vector>
#include<complex>
#include<algorithm>

using namespace std;
namespace matrix_operations
{
    //vec_product(m*1) = matrix(m*n) * vec(n*1)
    template <typename T>
    void matrix_vector_multiply(vector<vector<T > > matrix, vector<T> vec, vector<T> &vec_product)
    {
        auto size = vec.size();
        vector<T > vec_tmp(size,0);
        for (int i = 0; i < matrix.size(); ++i)
        {
            for (int j = 0; j < vec.size(); ++j)
            {
                vec_tmp[i] = vec_tmp[i] + matrix[i][j] * vec[j];
            }
        }
        vec_product.swap(vec_tmp);

    }


    template <typename T>
    void matrix_vector_multiply_mod(vector<vector<T > > matrix, vector<T> vec, vector<T> &vec_product,uint32_t mod)
    {
        auto size = vec.size();
        vector<T > vec_tmp(size,0);
        for (int i = 0; i < matrix.size(); ++i)
        {
            for (int j = 0; j < vec.size(); ++j)
            {
                vec_tmp[i] = ((uint64_t )vec_tmp[i] + (uint64_t )matrix[i][j] * (uint64_t )vec[j] % (uint64_t )mod) % mod;
            }
        }
        vec_product.swap(vec_tmp);

    }


//void matrix_vector_multiply(vector<vector< complex<double> >> matrix, vector< complex<double> > vec, vector< complex<double> > &vec_product);


//vec_sum = vec_a + vec_b
    void add(vector< complex<double> > vec_a, vector<complex<double>> vec_b, vector< complex<double> > &vec_sum);
    void multiply(vector< complex<double> > vec_a, vector<complex<double>> vec_b, vector< complex<double> > &vec_sum);

//matrix_sum = matrix_a + matrix_b
    void matrix_add(vector<vector< complex<double> >> matrix_a, vector<vector<complex<double> >> matrix_b, vector<vector< complex<double> >> &matrix_sum);


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
    void scalar_multiply(vector<vector< complex<double> >> matrix_a, double n, vector<vector< complex<double> >> &matrix_product);

//matrix_product = matrix_a * matrix_b
    void multiply(vector<vector<complex<double>>> matrix_a, vector<vector<complex<double> > > matrix_b, vector<vector<complex<double>>> &matrix_product);


//vec_diag = matrix_a.diagonal(diag_index)
    template <typename T>
    void diagonal(const vector<vector<T > > &matrix_a, size_t diag_index, vector<T > &vec_diag)
    {
        vector<T > vec(matrix_a.size());
        for(int i=0; i<matrix_a.size(); ++i)
        {
            vec[i] = matrix_a[i][(i+diag_index) % matrix_a[0].size()];
        }
        vec_diag.swap(vec);
    }

//vec_rotate = vec.rotate(rotation)
    template<typename T>
    void rotate(vector<T > vec, size_t rotation, vector<T > &vec_rotate)
    {

        auto size = vec.size();
        vector<T > vec_tmp(size);
        for(size_t i = 0; i < size; ++i)
        {
            vec_tmp[i] = vec[(i+rotation) % vec.size()];
        }
        vec_rotate.swap(vec_tmp);
    }

    template<typename T>
    vector<T > rotate(vector<T > vec, size_t rotation)
    {

        auto size = vec.size();
        vector<T > vec_tmp(size);
        for(size_t i = 0; i < size; ++i)
        {
            vec_tmp[i] = vec[(i+rotation) % vec.size()];
        }
        return vec_tmp;
    }

//matrix_conj = conjugate of matrix
    void conjugate_matrix(vector<vector<complex<double>>> matrix, vector<vector<complex<double>>> &matrix_conj);


//matrix_trans = transpose of matrix
//void transpose_matrix(const vector<vector<complex<double>>> &matrix, vector<vector<complex<double>>> &matrix_trans);
    template<typename T>
    void transpose_matrix(const vector<vector<T>> &matrix, vector<vector<T > > &matrix_trans)
    {
        vector<vector<T > > trans_matrix(matrix[0].size(),vector<T >(matrix.size()));
        for(int i=0; i<matrix[0].size(); ++i)
        {
            for(int j=0; j<matrix.size(); ++j)
            {
                //tmp.push_back(matrix[j][i]);
                trans_matrix[i][j] = matrix[j][i];
            }
            //trans_matrix.push_back(tmp);
        }
        matrix_trans.swap(trans_matrix);
    }


    //for bfv
    template<typename T>
    vector<T> rotate_slots_vec(const vector<T> &vec ,int k){
        auto size = vec.size();
        vector<T> ret(size);
        auto slots = size >> 1;

        vector<T > half_l(slots);
        vector<T > half_h(slots);
        copy_n(vec.cbegin(),slots,half_l.begin());
        copy_n(vec.cbegin()+slots,slots,half_h.begin());
//
        auto rot0 = rotate(half_l,k);
        auto rot1 = rotate(half_h,k);
        if(abs(k)  >= slots &&  abs(k) < size){
            copy_n(rot0.cbegin(), slots,ret.begin() + slots);
            copy_n(rot1.cbegin(), slots, ret.begin());
        }
        else if(abs(k) < slots){
            copy_n(rot0.cbegin(), slots,ret.begin());
            copy_n(rot1.cbegin(), slots, ret.begin() + slots);
        }
        else{
            throw invalid_argument("rotate out of range");
        }

        return ret;
    }

    template<typename T>
    void rotate_slots_matrix(const vector<vector<T>> &matrix, vector<vector<T > > &matrix_trans)
    {
        vector<vector<T > > trans_matrix(matrix[0].size(),vector<T >(matrix.size()));
        vector<vector<T > > trans_matrix_rot_slot(matrix[0].size(),vector<T >(matrix.size()));
        //transpose_matrix(matrix,trans_matrix);
        for(int i=0; i<matrix[0].size(); ++i) {

            trans_matrix_rot_slot[i] = rotate_slots_vec(matrix[i],i);

        }

        transpose_matrix(trans_matrix_rot_slot,matrix_trans);
        //matrix_trans.swap(trans_matrix_rot_slot);
    }



    void PrintVec(vector<vector<complex<double>>>& A);
} // namespace name
#endif
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