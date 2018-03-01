
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifndef SX_TYPE_DEF
#define SX_TYPE_DEF

#include "config.h"

#if USE_LONG_DOUBLE
typedef long double              SX_FLOAT;
#else
typedef double                   SX_FLOAT;
#endif

#if USE_LONG_LONG
typedef signed long long int     SX_INT;
#elif USE_LONG
typedef signed long int          SX_INT;
#else
typedef signed int               SX_INT;
#endif

/**
 * \struct SX_MAT
 * \brief Sparse matrix of SX_FLOAT type in CSR format
 *
 * CSR Format (Ap, Aj, Ax) in SX_FLOAT
 *
 * \note The starting index of A is 0.
 */
typedef struct SX_MAT
{
    SX_INT num_rows;
    SX_INT num_cols;
    SX_INT num_nnzs;

    SX_INT *Ap;
    SX_INT *Aj;
    SX_FLOAT *Ax;

} SX_MAT;

/**
 * \struct SX_MAT
 * \brief Sparse matrix of SX_INT type in CSR format
 *
 * CSR Format (Ap, Aj, Ax) in integer
 *
 * \note The starting index of A is 0.
 */
typedef struct SX_IMAT
{
    SX_INT num_rows;
    SX_INT num_cols;
    SX_INT num_nnzs;

    SX_INT *Ap;
    SX_INT *Aj;
    SX_INT *Ax;

} SX_IMAT;

/**
 * \struct SX_VEC
 * \brief Vector with n entries of SX_FLOAT type
 */
typedef struct SX_VEC 
{
    SX_INT n;
    SX_FLOAT *d;

} SX_VEC;

/**
 * \struct SX_IVEC
 * \brief Vector with n entries of SX_INT type
 */
typedef struct SX_IVEC
{
    SX_INT n;
    SX_INT *d;

} SX_IVEC;

/**
 * \brief Definition of standard smoother types
 */
typedef enum
{
    SX_SM_JACOBI    = 1,  /**< Jacobi smoother */
    SX_SM_GS        = 2,  /**< Gauss-Seidel smoother */
    SX_SM_SGS       = 3,  /**< Symmetric Gauss-Seidel smoother */
    SX_SM_SOR       = 4,  /**< SOR smoother */
    SX_SM_SSOR      = 5,  /**< SSOR smoother */
    SX_SM_GSOR      = 6,  /**< GS + SOR smoother */
    SX_SM_SGSOR     = 7,  /**< SGS + SSOR smoother */
    SX_SM_POLY      = 8,  /**< Polynomial smoother */
    SX_SM_L1DIAG    = 9,  /**< L1 norm diagonal scaling smoother */

} SX_SM_TYPE;

/**
 * \brief Definition of coarsening types
 */
typedef enum
{
    SX_COARSE_RS      = 1,  /**< Classical */
    SX_COARSE_RSP     = 2,  /**< Classical, with positive offdiags */

} SX_COARSEN_TYPE;

/**
 * \brief Definition of interpolation types
 */
typedef enum
{
    SX_INTERP_DIR     = 1,  /**< Direct interpolation */
    SX_INTERP_STD     = 2,  /**< Standard interpolation */

} SX_INTERP_TYPE;

typedef struct SX_SMTR
{
    SX_SM_TYPE smoother;

    SX_MAT *A;
    SX_VEC *b;
    SX_VEC *x;

    SX_FLOAT relax;
    SX_INT nsweeps;
    SX_INT istart;
    SX_INT iend;
    SX_INT istep;
    SX_INT ndeg;
    SX_INT cf_order;
    SX_INT *ordering;

} SX_SMTR;

/**
 * \struct SX_AMG_PARS
 * \brief Parameters for AMG solver
 *
 */
typedef struct SX_AMG_PARS
{
    SX_INT verb;

    SX_INT   cycle_itr;          /** type of AMG cycle, 0, 1 is for V, others for W */
    SX_FLOAT tol;                /** stopping tolerance for AMG solver */
    SX_FLOAT ctol;               /** stopping tolerance for coarsest solver */
    SX_INT   maxit;              /** max number of iterations of AMG */

    SX_COARSEN_TYPE cs_type;     /** coarsening type */
    SX_INT max_levels;           /** max number of levels of AMG */
    SX_INT coarse_dof;           /** max number of coarsest level DOF */

    SX_SM_TYPE smoother;         /** smoother type */
    SX_FLOAT relax;              /** relax parseter for SOR smoother */
    SX_INT cf_order;             /** False: nature order True: C/F order */
    SX_INT pre_iter;             /** number of presmoothers */
    SX_INT post_iter;            /** number of postsmoothers */
    SX_INT poly_deg;             /** degree of the polynomial smoother */

    SX_INTERP_TYPE interp_type;  /** interpolation type */
    SX_FLOAT strong_threshold;   /** strong connection threshold for coarsening */
    SX_FLOAT max_row_sum;        /** maximal row sum parseter */
    SX_FLOAT trunc_threshold;    /** truncation threshold */

} SX_AMG_PARS;

/* return values */
typedef struct SX_RTN
{
    SX_FLOAT ares;     /* absolute residual */
    SX_FLOAT rres;     /* relative residual */
    SX_INT   nits;     /* number of iterations */

} SX_RTN;

/** 
 * \struct SX_AMG 
 * \brief Data for AMG solvers
 * 
 */ 
typedef struct SX_AMG
{
    SX_INT max_levels;            /** max number of levels */
    SX_INT num_levels;            /** number of levels in use <= max_levels */

    SX_MAT A;                    /** pointer to the matrix at level level_num */
    SX_MAT R;                    /** restriction operator at level level_num */
    SX_MAT P;                    /** prolongation operator at level level_num */
    SX_VEC b;                    /** pointer to the right-hand side at level level_num */
    SX_VEC x;                    /** pointer to the iterative solution at level level_num */
    SX_IVEC cfmark;               /** pointer to the CF marker at level level_num */

    SX_VEC     wp;               /** cache work space */
    SX_RTN      rtn;              /* return values */
    SX_AMG_PARS pars;             /* AMG parameters */

} SX_AMG;

typedef struct SX_KRYLOV
{
    SX_FLOAT tol;
    SX_MAT *A;
    SX_VEC *b;
    SX_VEC *u;
    SX_INT restart;
    SX_INT maxit;
    SX_INT stop_type;
    SX_INT verb;

} SX_KRYLOV;

#endif
