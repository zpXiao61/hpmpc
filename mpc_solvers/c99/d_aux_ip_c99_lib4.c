/**************************************************************************************************
*                                                                                                 *
* This file is part of HPMPC.                                                                     *
*                                                                                                 *
* HPMPC -- Library for High-Performance implementation of solvers for MPC.                        *
* Copyright (C) 2014 by Technical University of Denmark. All rights reserved.                     *
*                                                                                                 *
* HPMPC is free software; you can redistribute it and/or                                          *
* modify it under the terms of the GNU Lesser General Public                                      *
* License as published by the Free Software Foundation; either                                    *
* version 2.1 of the License, or (at your option) any later version.                              *
*                                                                                                 *
* HPMPC is distributed in the hope that it will be useful,                                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                                            *
* See the GNU Lesser General Public License for more details.                                     *
*                                                                                                 *
* You should have received a copy of the GNU Lesser General Public                                *
* License along with HPMPC; if not, write to the Free Software                                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA                  *
*                                                                                                 *
* Author: Gianluca Frison, giaf (at) dtu.dk                                                       *
*                                                                                                 *
**************************************************************************************************/

#include "../../include/block_size.h"



void d_init_var_box_mpc(int N, int nx, int nu, int nb, double **ux, double **pi, double **db, double **t, double **lam, int warm_start)
	{

	const int nbu = nu<nb ? nu : nb ;
	
	int jj, ll, ii;
	
	double thr0 = 1e-3; // minimum distance from a constraint

	if(warm_start==1)
		{
		for(ll=0; ll<2*nbu; ll+=2)
			{
			t[0][ll+0] =   ux[0][ll/2] - db[0][ll+0];
			t[0][ll+1] = - db[0][ll+1] - ux[0][ll/2];
			if(t[0][ll+0] < thr0)
				{
				if(t[0][ll+1] < thr0)
					{
					ux[0][ll/2] = ( - db[0][ll+1] + db[0][ll+0])*0.5;
					t[0][ll+0] =   ux[0][ll/2] - db[0][ll+0];
					t[0][ll+1] = - db[0][ll+1] - ux[0][ll/2];
					}
				else
					{
					t[0][ll+0] = thr0;
					ux[0][ll/2] = db[0][ll+0] + thr0;
					}
				}
			else if(t[0][ll+1] < thr0)
				{
				t[0][ll+1] = thr0;
				ux[0][ll/2] = - db[0][ll+1] - thr0;
				}
			lam[0][ll+0] = 1.0/t[0][ll+0];
			lam[0][ll+1] = 1.0/t[0][ll+1];
			}
		for(; ll<2*nb; ll++)
			{
			t[0][ll] = 1.0; // this has to be strictly positive !!!
			lam[0][ll] = 1.0; // this has to be strictly positive !!!
			}
		for(jj=1; jj<N; jj++)
			{
			for(ll=0; ll<2*nb; ll+=2)
				{
				t[jj][ll+0] = ux[jj][ll/2] - db[jj][ll+0];
				t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
				if(t[jj][ll+0] < thr0)
					{
					if(t[jj][ll+1] < thr0)
						{
						ux[jj][ll/2] = ( - db[jj][ll+1] + db[jj][ll+0])*0.5;
						t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
						t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
						}
					else
						{
						t[jj][ll+0] = thr0;
						ux[jj][ll/2] = db[jj][ll+0] + thr0;
						}
					}
				else if(t[jj][ll+1] < thr0)
					{
					t[jj][ll+1] = thr0;
					ux[jj][ll/2] = - db[jj][ll+1] - thr0;
					}
				lam[jj][ll+0] = 1.0/t[jj][ll+0];
				lam[jj][ll+1] = 1.0/t[jj][ll+1];
				}
			}
		for(ll=0; ll<2*nbu; ll++) // this has to be strictly positive !!!
			{
			t[N][ll] = 1.0;
			lam[N][ll] = 1.0;
			}
		for(ll=2*nu; ll<2*nb; ll+=2)
			{
			t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
			t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
			if(t[N][ll+0] < thr0)
				{
				if(t[N][ll+1] < thr0)
					{
					ux[N][ll/2] = ( - db[N][ll+1] + db[N][ll+0])*0.5;
					t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
					t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
					}
				else
					{
					t[N][ll+0] = thr0;
					ux[N][ll/2] = db[N][ll+0] + thr0;
					}
				}
			else if(t[N][ll+1] < thr0)
				{
				t[N][ll+1] = thr0;
				ux[N][ll/2] = - db[N][ll+1] - thr0;
				}
			lam[N][ll+0] = 1.0/t[N][ll+0];
			lam[N][ll+1] = 1.0/t[N][ll+1];
			}
		}
	else // cold start
		{
		for(ll=0; ll<2*nbu; ll+=2)
			{
			ux[0][ll/2] = 0.0;
/*			t[0][ll+0] = 1.0;*/
/*			t[0][ll+1] = 1.0;*/
			t[0][ll+0] =   ux[0][ll/2] - db[0][ll+0];
			t[0][ll+1] = - db[0][ll+1] - ux[0][ll/2];
			if(t[0][ll+0] < thr0)
				{
				if(t[0][ll+1] < thr0)
					{
					ux[0][ll/2] = ( - db[0][ll+1] + db[0][ll+0])*0.5;
					t[0][ll+0] =   ux[0][ll/2] - db[0][ll+0];
					t[0][ll+1] = - db[0][ll+1] - ux[0][ll/2];
					}
				else
					{
					t[0][ll+0] = thr0;
					ux[0][ll/2] = db[0][ll+0] + thr0;
					}
				}
			else if(t[0][ll+1] < thr0)
				{
				t[0][ll+1] = thr0;
				ux[0][ll/2] = - db[0][ll+1] - thr0;
				}
			lam[0][ll+0] = 1.0/t[0][ll+0];
			lam[0][ll+1] = 1.0/t[0][ll+1];
			}
		for(ii=ll/2; ii<nu; ii++)
			ux[0][ii] = 0.0; // initialize remaining components of u to zero
		for(; ll<2*nb; ll++)
			{
			t[0][ll] = 1.0; // this has to be strictly positive !!!
			lam[0][ll] = 1.0; // this has to be strictly positive !!!
			}
		for(jj=1; jj<N; jj++)
			{
			for(ll=0; ll<2*nb; ll+=2)
				{
				ux[jj][ll/2] = 0.0;
/*				t[jj][ll+0] = 1.0;*/
/*				t[jj][ll+1] = 1.0;*/
				t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
				t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
				if(t[jj][ll+0] < thr0)
					{
					if(t[jj][ll+1] < thr0)
						{
						ux[jj][ll/2] = ( - db[jj][ll+1] + db[jj][ll+0])*0.5;
						t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
						t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
						}
					else
						{
						t[jj][ll+0] = thr0;
						ux[jj][ll/2] = db[jj][ll+0] + thr0;
						}
					}
				else if(t[jj][ll+1] < thr0)
					{
					t[jj][ll+1] = thr0;
					ux[jj][ll/2] = - db[jj][ll+1] - thr0;
					}
				lam[jj][ll+0] = 1.0/t[jj][ll+0];
				lam[jj][ll+1] = 1.0/t[jj][ll+1];
				}
			for(ii=ll/2; ii<nx+nu; ii++)
				ux[jj][ii] = 0.0; // initialize remaining components of u and x to zero
			}
		for(ll=0; ll<2*nbu; ll++)
			{
			t[N][ll] = 1.0; // this has to be strictly positive !!!
			lam[N][ll] = 1.0; // this has to be strictly positive !!!
			}
		for(ll=2*nu; ll<2*nb; ll+=2)
			{
			ux[N][ll/2] = 0.0;
/*			t[N][ll+0] = 1.0;*/
/*			t[N][ll+1] = 1.0;*/
			t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
			t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
			if(t[N][ll+0] < thr0)
				{
				if(t[N][ll+1] < thr0)
					{
					ux[N][ll/2] = ( - db[N][ll+1] + db[N][ll+0])*0.5;
					t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
					t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
					}
				else
					{
					t[N][ll+0] = thr0;
					ux[N][ll/2] = db[N][ll+0] + thr0;
					}
				}
			else if(t[N][ll+1] < thr0)
				{
				t[N][ll+1] = thr0;
				ux[N][ll/2] = - db[N][ll+1] - thr0;
				}
			lam[N][ll+0] = 1.0/t[N][ll+0];
			lam[N][ll+1] = 1.0/t[N][ll+1];
			}
		for(ii=ll/2; ii<nx+nu; ii++)
			ux[N][ii] = 0.0; // initialize remaining components of x to zero

		for(jj=0; jj<=N; jj++)
			for(ll=0; ll<nx; ll++)
				pi[jj][ll] = 0.0; // initialize multipliers to zero

		}
	
	}



void d_init_var_soft_mpc(int N, int nx, int nu, int nb, double **ux, double **pi, double **db, double **t, double **lam, int warm_start)
	{
	
	const int nbu = nu<nb ? nu : nb ;
	const int nbx = nb-nu>0 ? nb-nu : 0 ;

	// constants
	const int bs = D_MR;
	const int ncl = D_NCL;
	const int nal = bs*ncl; // number of doubles per cache line

	const int anb = nal*((2*nb+nal-1)/nal); // cache aligned number of box and soft constraints // !!!!! doubled to include soft constraints !!!!!

	int jj, ll, ii;
	
	double thr0 = 1e-3; // minimum distance from a constraint

	if(warm_start==1)
		{
		for(ll=0; ll<2*nbu; ll+=2)
			{
			t[0][ll+0] =   ux[0][ll/2] - db[0][ll+0];
			t[0][ll+1] = - db[0][ll+1] - ux[0][ll/2];
			if(t[0][ll+0] < thr0)
				{
				if(t[0][ll+1] < thr0)
					{
					ux[0][ll/2] = ( - db[0][ll+1] + db[0][ll+0])*0.5;
					t[0][ll+0] =   ux[0][ll/2] - db[0][ll+0];
					t[0][ll+1] = - db[0][ll+1] - ux[0][ll/2];
					}
				else
					{
					t[0][ll+0] = thr0;
					ux[0][ll/2] = db[0][ll+0] + thr0;
					}
				}
			else if(t[0][ll+1] < thr0)
				{
				t[0][ll+1] = thr0;
				ux[0][ll/2] = - db[0][ll+1] - thr0;
				}

			lam[0][ll+0] = 1.0/t[0][ll+0];
			lam[0][ll+1] = 1.0/t[0][ll+1];
			}
		for(; ll<2*nb; ll++)
			{
			t[0][ll] = 1.0; // this has to be strictly positive !!!
			lam[0][ll] = 1.0;
			}
		for(jj=1; jj<N; jj++)
			{
			for(ll=0; ll<2*nb; ll+=2)
				{
				t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
				t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
				if(t[jj][ll+0] < thr0)
					{
					if(t[jj][ll+1] < thr0)
						{
						ux[jj][ll/2] = ( - db[jj][ll+1] + db[jj][ll+0])*0.5;
						t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
						t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
						}
					else
						{
						t[jj][ll+0] = thr0;
						ux[jj][ll/2] = db[jj][ll+0] + thr0;
						}
					}
				else if(t[jj][ll+1] < thr0)
					{
					t[jj][ll+1] = thr0;
					ux[jj][ll/2] = - db[jj][ll+1] - thr0;
					}
				lam[jj][ll+0] = 1.0/t[jj][ll+0];
				lam[jj][ll+1] = 1.0/t[jj][ll+1];
				}
			}
		for(ll=0; ll<2*nbu; ll++) // this has to be strictly positive !!!
			{
			t[N][ll] = 1.0;
			lam[N][ll] = 1.0;
			}
		for(ll=2*nu; ll<2*nb; ll+=2)
			{
			t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
			t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
			if(t[N][ll+0] < thr0)
				{
				if(t[N][ll+1] < thr0)
					{
					ux[N][ll/2] = ( - db[N][ll+1] + db[N][ll+0])*0.5;
					t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
					t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
					}
				else
					{
					t[N][ll+0] = thr0;
					ux[N][ll/2] = db[N][ll+0] + thr0;
					}
				}
			else if(t[N][ll+1] < thr0)
				{
				t[N][ll+1] = thr0;
				ux[N][ll/2] = - db[N][ll+1] - thr0;
				}
			lam[N][ll+0] = 1.0/t[N][ll+0];
			lam[N][ll+1] = 1.0/t[N][ll+1];
			}

		}
	else // cold start
		{
		for(ll=0; ll<2*nbu; ll+=2)
			{
			ux[0][ll/2] = 0.0;
/*			t[0][ll+0] = 1.0;*/
/*			t[0][ll+1] = 1.0;*/
			t[0][ll+0] =   ux[0][ll/2] - db[0][ll+0];
			t[0][ll+1] = - db[0][ll+1] - ux[0][ll/2];
			if(t[0][ll+0] < thr0)
				{
				if(t[0][ll+1] < thr0)
					{
					ux[0][ll/2] = ( - db[0][ll+1] + db[0][ll+0])*0.5;
					t[0][ll+0] =   ux[0][ll/2] - db[0][ll+0];
					t[0][ll+1] = - db[0][ll+1] - ux[0][ll/2];
					}
				else
					{
					t[0][ll+0] = thr0;
					ux[0][ll/2] = db[0][ll+0] + thr0;
					}
				}
			else if(t[0][ll+1] < thr0)
				{
				t[0][ll+1] = thr0;
				ux[0][ll/2] = - db[0][ll+1] - thr0;
				}
			lam[0][ll+0] = 1.0/t[0][ll+0];
			lam[0][ll+1] = 1.0/t[0][ll+1];
			}
		for(ii=ll/2; ii<nu; ii++)
			ux[0][ii] = 0.0; // initialize remaining components of u to zero
		for(; ll<2*nb; ll++)
			{
			t[0][ll] = 1.0; // this has to be strictly positive !!!
			lam[0][ll] = 1.0;
			}
		for(jj=1; jj<N; jj++)
			{
			for(ll=0; ll<2*nb; ll+=2)
				{
				ux[jj][ll/2] = 0.0;
/*				t[jj][ll+0] = 1.0;*/
/*				t[jj][ll+1] = 1.0;*/
				t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
				t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
				if(t[jj][ll+0] < thr0)
					{
					if(t[jj][ll+1] < thr0)
						{
						ux[jj][ll/2] = ( - db[jj][ll+1] + db[jj][ll+0])*0.5;
						t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
						t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
						}
					else
						{
						t[jj][ll+0] = thr0;
						ux[jj][ll/2] = db[jj][ll+0] + thr0;
						}
					}
				else if(t[jj][ll+1] < thr0)
					{
					t[jj][ll+1] = thr0;
					ux[jj][ll/2] = - db[jj][ll+1] - thr0;
					}
				lam[jj][ll+0] = 1.0/t[jj][ll+0];
				lam[jj][ll+1] = 1.0/t[jj][ll+1];
				}
			for(ii=ll/2; ii<nx+nu; ii++)
				ux[jj][ii] = 0.0; // initialize remaining components of u and x to zero
			}
		for(ll=0; ll<2*nbu; ll++)
			{
			t[N][ll] = 1.0; // this has to be strictly positive !!!
			lam[N][ll] = 1.0;
			}
		for(ll=2*nu; ll<2*nb; ll+=2)
			{
			ux[N][ll/2] = 0.0;
/*			t[N][ll+0] = 1.0;*/
/*			t[N][ll+1] = 1.0;*/
			t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
			t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
			if(t[N][ll+0] < thr0)
				{
				if(t[N][ll+1] < thr0)
					{
					ux[N][ll/2] = ( - db[N][ll+1] + db[N][ll+0])*0.5;
					t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
					t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
					}
				else
					{
					t[N][ll+0] = thr0;
					ux[N][ll/2] = db[N][ll+0] + thr0;
					}
				}
			else if(t[N][ll+1] < thr0)
				{
				t[N][ll+1] = thr0;
				ux[N][ll/2] = - db[N][ll+1] - thr0;
				}
			lam[N][ll+0] = 1.0/t[N][ll+0];
			lam[N][ll+1] = 1.0/t[N][ll+1];
			}
		for(ii=ll/2; ii<nx+nu; ii++)
			ux[N][ii] = 0.0; // initialize remaining components of x to zero

		// inizialize t_theta (cold start only for the moment)
		for(jj=0; jj<=N; jj++)
			for(ll=0; ll<2*nbx; ll++)
				t[jj][anb+2*nu+ll] = 1.0;

		// initialize lam_theta (cold start only for the moment)
		for(jj=0; jj<=N; jj++)
			for(ll=0; ll<2*nbx; ll++)
				lam[jj][anb+2*nu+ll] = 1.0;

		// initialize pi
		for(jj=0; jj<=N; jj++)
			for(ll=0; ll<nx; ll++)
				pi[jj][ll] = 0.0; // initialize multipliers to zero

		}
	
	}



void d_update_hessian_box_mpc(int N, int nx, int nu, int nb, int cnz, double sigma_mu, double **t, double **t_inv, double **lam, double **lamt, double **dlam, double **bd, double **bl, double **pd, double **pl, double **pl2, double **db)

/*void d_update_hessian_box(int k0, int kmax, int nb, int cnz, double sigma_mu, double *t, double *lam, double *lamt, double *dlam, double *bd, double *bl, double *pd, double *pl, double *lb, double *ub)*/
	{
	// TODO remove k1 !!! and use nx nu nb instead of k0 k1 kmax
	
	const int nbu = nu<nb ? nu : nb ;

	const int bs = 4; //d_get_mr();

	//const int k0 = nbu;
	//const int k1 = (nu/bs)*bs;
	//const int kmax = nb;
	
	double temp0, temp1;
	
	double *ptr_t, *ptr_lam, *ptr_lamt, *ptr_dlam, *ptr_tinv, *ptr_pd, *ptr_pl, *ptr_pl2, *ptr_bd, *ptr_bl, *ptr_db;
	
	int ii, jj, ll, bs0;
	
	// first stage
	
	ptr_t     = t[0];
	ptr_lam   = lam[0];
	ptr_lamt  = lamt[0];
	ptr_dlam  = dlam[0];
	ptr_tinv  = t_inv[0];
	ptr_pd    = pd[0];
	ptr_pl    = pl[0];
	ptr_pl2   = pl2[0];
	ptr_bd    = bd[0];
	ptr_bl    = bl[0];
	ptr_db    = db[0];
	
	ii = 0;
	for(; ii<nbu-3; ii+=4)
		{

		ptr_tinv[0] = 1.0/ptr_t[0];
		ptr_tinv[1] = 1.0/ptr_t[1];
		ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
		ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
		ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
		ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
		ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + ptr_lamt[0] + ptr_lamt[1];
		ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
		ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

		ptr_tinv[2] = 1.0/ptr_t[2];
		ptr_tinv[3] = 1.0/ptr_t[3];
		ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
		ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
		ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
		ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
		ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + ptr_lamt[2] + ptr_lamt[3];
		ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + ptr_lam[3] + ptr_lamt[3]*ptr_db[3] + ptr_dlam[3] - ptr_lam[2] - ptr_lamt[2]*ptr_db[2] - ptr_dlam[2];
		ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

		ptr_tinv[4] = 1.0/ptr_t[4];
		ptr_tinv[5] = 1.0/ptr_t[5];
		ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
		ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
		ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
		ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
		ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + ptr_lamt[4] + ptr_lamt[5];
		ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + ptr_lam[5] + ptr_lamt[5]*ptr_db[5] + ptr_dlam[5] - ptr_lam[4] - ptr_lamt[4]*ptr_db[4] - ptr_dlam[4];
		ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

		ptr_tinv[6] = 1.0/ptr_t[6];
		ptr_tinv[7] = 1.0/ptr_t[7];
		ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
		ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
		ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
		ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
		ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + ptr_lamt[6] + ptr_lamt[7];
		ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + ptr_lam[7] + ptr_lamt[7]*ptr_db[7] + ptr_dlam[7] - ptr_lam[6] - ptr_lamt[6]*ptr_db[6] - ptr_dlam[6];
		ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

		ptr_t     += 8;
		ptr_lam   += 8;
		ptr_lamt  += 8;
		ptr_dlam  += 8;
		ptr_tinv  += 8;
		ptr_db    += 8;

		}
	if(ii<nbu)
		{
		bs0 = nbu-ii;
		for(ll=0; ll<bs0; ll++)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
/*ptr_dlam[0] = 1;*/
/*printf("\n%d\n", k0);*/
/*return;*/
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + ptr_lamt[0] + ptr_lamt[1];
			ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
			ptr_pl2[ii+ll+0] = ptr_pl[(ii+ll)*bs];

			ptr_t     += 2;
			ptr_lam   += 2;
			ptr_lamt  += 2;
			ptr_dlam  += 2;
			ptr_tinv  += 2;
			ptr_db    += 2;
			}
		}

	// middle stages

	for(jj=1; jj<N; jj++)
		{
		
		ptr_t     = t[jj];
		ptr_lam   = lam[jj];
		ptr_lamt  = lamt[jj];
		ptr_dlam  = dlam[jj];
		ptr_tinv  = t_inv[jj];
		ptr_pd    = pd[jj];
		ptr_pl    = pl[jj];
		ptr_pl2   = pl2[jj];
		ptr_bd    = bd[jj];
		ptr_bl    = bl[jj];
		ptr_db    = db[jj];

		ii = 0;
		for(; ii<nb-3; ii+=4)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + ptr_lamt[0] + ptr_lamt[1];
			ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
			ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

			ptr_tinv[2] = 1.0/ptr_t[2];
			ptr_tinv[3] = 1.0/ptr_t[3];
			ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
			ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
			ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
			ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
			ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + ptr_lamt[2] + ptr_lamt[3];
			ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + ptr_lam[3] + ptr_lamt[3]*ptr_db[3] + ptr_dlam[3] - ptr_lam[2] - ptr_lamt[2]*ptr_db[2] - ptr_dlam[2];
			ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

			ptr_tinv[4] = 1.0/ptr_t[4];
			ptr_tinv[5] = 1.0/ptr_t[5];
			ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
			ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
			ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
			ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
			ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + ptr_lamt[4] + ptr_lamt[5];
			ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + ptr_lam[5] + ptr_lamt[5]*ptr_db[5] + ptr_dlam[5] - ptr_lam[4] - ptr_lamt[4]*ptr_db[4] - ptr_dlam[4];
			ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

			ptr_tinv[6] = 1.0/ptr_t[6];
			ptr_tinv[7] = 1.0/ptr_t[7];
			ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
			ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
			ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
			ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
			ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + ptr_lamt[6] + ptr_lamt[7];
			ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + ptr_lam[7] + ptr_lamt[7]*ptr_db[7] + ptr_dlam[7] - ptr_lam[6] - ptr_lamt[6]*ptr_db[6] - ptr_dlam[6];
			ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

			ptr_t     += 8;
			ptr_lam   += 8;
			ptr_lamt  += 8;
			ptr_dlam  += 8;
			ptr_tinv  += 8;
			ptr_db    += 8;

			}
		if(ii<nb)
			{
			bs0 = nb-ii;
			for(ll=0; ll<bs0; ll++)
				{
				ptr_tinv[0] = 1.0/ptr_t[0];
				ptr_tinv[1] = 1.0/ptr_t[1];
				ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
				ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
				ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
				ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
				ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + ptr_lamt[0] + ptr_lamt[1];
				ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
				ptr_pl2[ii+ll+0] = ptr_pl[(ii+ll)*bs];

				ptr_t     += 2;
				ptr_lam   += 2;
				ptr_lamt  += 2;
				ptr_dlam  += 2;
				ptr_tinv  += 2;
				ptr_db    += 2;
				}
			}
	
		}

	// last stage

	ptr_t     = t[N]     + 2*nu;
	ptr_lam   = lam[N]   + 2*nu;
	ptr_lamt  = lamt[N]  + 2*nu;
	ptr_dlam  = dlam[N]  + 2*nu;
	ptr_tinv  = t_inv[N] + 2*nu;
	ptr_pd    = pd[N];
	ptr_pl    = pl[N];
	ptr_pl2   = pl2[N];
	ptr_bd    = bd[N];
	ptr_bl    = bl[N];
	ptr_db    = db[N];

	ii=4*(nu/4); // k1 supposed to be multiple of bs !!!!!!!!!! NO MORE !!!!!!!
	if(ii<nu)
		{
		bs0 = nb-ii<4 ? nb-ii : 4 ;
		ll = nu-ii; //k0%4;
		for(; ll<bs0; ll++)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + ptr_lamt[0] + ptr_lamt[1];
			ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
			ptr_pl2[ii+ll+0] = ptr_pl[(ii+ll)*bs];

			ptr_t     += 2;
			ptr_lam   += 2;
			ptr_lamt  += 2;
			ptr_dlam  += 2;
			ptr_tinv  += 2;
			ptr_db    += 2;
			}
		ii += 4;
		}

	for(; ii<nb-3; ii+=4)
		{
		ptr_tinv[0] = 1.0/ptr_t[0];
		ptr_tinv[1] = 1.0/ptr_t[1];
		ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
		ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
		ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
		ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
		ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + ptr_lamt[0] + ptr_lamt[1];
		ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
		ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

		ptr_tinv[2] = 1.0/ptr_t[2];
		ptr_tinv[3] = 1.0/ptr_t[3];
		ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
		ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
		ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
		ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
		ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + ptr_lamt[2] + ptr_lamt[3];
		ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + ptr_lam[3] + ptr_lamt[3]*ptr_db[3] + ptr_dlam[3] - ptr_lam[2] - ptr_lamt[2]*ptr_db[2] - ptr_dlam[2];
		ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

		ptr_tinv[4] = 1.0/ptr_t[4];
		ptr_tinv[5] = 1.0/ptr_t[5];
		ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
		ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
		ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
		ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
		ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + ptr_lamt[4] + ptr_lamt[5];
		ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + ptr_lam[5] + ptr_lamt[5]*ptr_db[5] + ptr_dlam[5] - ptr_lam[4] - ptr_lamt[4]*ptr_db[4] - ptr_dlam[4];
		ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

		ptr_tinv[6] = 1.0/ptr_t[6];
		ptr_tinv[7] = 1.0/ptr_t[7];
		ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
		ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
		ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
		ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
		ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + ptr_lamt[6] + ptr_lamt[7];
		ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + ptr_lam[7] + ptr_lamt[7]*ptr_db[7] + ptr_dlam[7] - ptr_lam[6] - ptr_lamt[6]*ptr_db[6] - ptr_dlam[6];
		ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

		ptr_t     += 8;
		ptr_lam   += 8;
		ptr_lamt  += 8;
		ptr_dlam  += 8;
		ptr_tinv  += 8;
		ptr_db    += 8;

		}
	if(ii<nb)
		{
		bs0 = nb-ii;
		for(ll=0; ll<bs0; ll++)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + ptr_lamt[0] + ptr_lamt[1];
			ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
			ptr_pl2[ii+ll+0] = ptr_pl[(ii+ll)*bs];

			ptr_t     += 2;
			ptr_lam   += 2;
			ptr_lamt  += 2;
			ptr_dlam  += 2;
			ptr_tinv  += 2;
			ptr_db    += 2;
			}
		}


	}



void d_update_hessian_soft_mpc(int N, int nx, int nu, int nb, int cnz, double sigma_mu, double **t, double **t_inv, double **lam, double **lamt, double **dlam, double **bd, double **bl, double **pd, double **pl, double **pl2, double **db, double **Z, double **z, double **Zl, double **zl)

/*void d_update_hessian_box(int k0, int kmax, int nb, int cnz, double sigma_mu, double *t, double *lam, double *lamt, double *dlam, double *bd, double *bl, double *pd, double *pl, double *lb, double *ub)*/
	{

	const int nbu = nu<nb ? nu : nb ;
	const int nbx = nb-nu>0 ? nb-nu : 0 ;

	// constants
	const int bs = 4; //D_MR;
	const int ncl = D_NCL;
	const int nal = bs*ncl; // number of doubles per cache line

	const int anb = nal*((2*nb+nal-1)/nal); // cache aligned number of box and soft constraints // !!!!! doubled to include soft constraints !!!!!

	//const int k0 = nbu;
	//const int k1 = (nu/bs)*bs;
	//const int kmax = nb;
	
	
	double temp0, temp1;
	
	double *ptr_t, *ptr_lam, *ptr_lamt, *ptr_dlam, *ptr_tinv, *ptr_pd, *ptr_pl, *ptr_pl2, *ptr_bd, *ptr_bl, *ptr_db, *ptr_Z, *ptr_z, *ptr_Zl, *ptr_zl;

	static double Qx[8] = {};
	static double qx[8] = {};
	
	int ii, jj, ll, bs0;
	
	// first stage
	
	ptr_t     = t[0];
	ptr_lam   = lam[0];
	ptr_lamt  = lamt[0];
	ptr_dlam  = dlam[0];
	ptr_tinv  = t_inv[0];
	ptr_pd    = pd[0];
	ptr_pl    = pl[0];
	ptr_pl2   = pl2[0];
	ptr_bd    = bd[0];
	ptr_bl    = bl[0];
	ptr_db    = db[0];
	//ptr_Z     = Z[0];
	//ptr_z     = z[0];
	//ptr_Zl    = Zl[0];
	//ptr_zl    = zl[0];
	
	ii = 0;
	// hard constraints on u only
	for(; ii<nbu-3; ii+=4)
		{

		ptr_tinv[0] = 1.0/ptr_t[0];
		ptr_tinv[1] = 1.0/ptr_t[1];
		ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
		ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
		ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
		ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
		ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + ptr_lamt[0] + ptr_lamt[1];
		ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
		ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

		ptr_tinv[2] = 1.0/ptr_t[2];
		ptr_tinv[3] = 1.0/ptr_t[3];
		ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
		ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
		ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
		ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
		ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + ptr_lamt[2] + ptr_lamt[3];
		ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + ptr_lam[3] + ptr_lamt[3]*ptr_db[3] + ptr_dlam[3] - ptr_lam[2] - ptr_lamt[2]*ptr_db[2] - ptr_dlam[2];
		ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

		ptr_tinv[4] = 1.0/ptr_t[4];
		ptr_tinv[5] = 1.0/ptr_t[5];
		ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
		ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
		ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
		ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
		ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + ptr_lamt[4] + ptr_lamt[5];
		ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + ptr_lam[5] + ptr_lamt[5]*ptr_db[5] + ptr_dlam[5] - ptr_lam[4] - ptr_lamt[4]*ptr_db[4] - ptr_dlam[4];
		ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

		ptr_tinv[6] = 1.0/ptr_t[6];
		ptr_tinv[7] = 1.0/ptr_t[7];
		ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
		ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
		ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
		ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
		ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + ptr_lamt[6] + ptr_lamt[7];
		ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + ptr_lam[7] + ptr_lamt[7]*ptr_db[7] + ptr_dlam[7] - ptr_lam[6] - ptr_lamt[6]*ptr_db[6] - ptr_dlam[6];
		ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

		ptr_t     += 8;
		ptr_lam   += 8;
		ptr_lamt  += 8;
		ptr_dlam  += 8;
		ptr_tinv  += 8;
		ptr_db    += 8;

		}
	if(ii<nbu)
		{
		bs0 = nbu-ii;
		for(ll=0; ll<bs0; ll++)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + ptr_lamt[0] + ptr_lamt[1];
			ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
			ptr_pl2[ii+ll+0] = ptr_pl[(ii+ll)*bs];

			ptr_t     += 2;
			ptr_lam   += 2;
			ptr_lamt  += 2;
			ptr_dlam  += 2;
			ptr_tinv  += 2;
			ptr_db    += 2;
			}
		}

	// middle stages

	for(jj=1; jj<N; jj++)
		{
		
		ptr_t     = t[jj];
		ptr_lam   = lam[jj];
		ptr_lamt  = lamt[jj];
		ptr_dlam  = dlam[jj];
		ptr_tinv  = t_inv[jj];
		ptr_pd    = pd[jj];
		ptr_pl    = pl[jj];
		ptr_pl2   = pl2[jj];
		ptr_bd    = bd[jj];
		ptr_bl    = bl[jj];
		ptr_db    = db[jj];
		ptr_Z     = Z[jj];
		ptr_z     = z[jj];
		ptr_Zl    = Zl[jj];
		ptr_zl    = zl[jj];

		ii = 0;
		// hard constraints on u
		for(; ii<nbu-3; ii+=4)
			{

			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + ptr_lamt[0] + ptr_lamt[1];
			ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
			ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

			ptr_tinv[2] = 1.0/ptr_t[2];
			ptr_tinv[3] = 1.0/ptr_t[3];
			ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
			ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
			ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
			ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
			ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + ptr_lamt[2] + ptr_lamt[3];
			ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + ptr_lam[3] + ptr_lamt[3]*ptr_db[3] + ptr_dlam[3] - ptr_lam[2] - ptr_lamt[2]*ptr_db[2] - ptr_dlam[2];
			ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

			ptr_tinv[4] = 1.0/ptr_t[4];
			ptr_tinv[5] = 1.0/ptr_t[5];
			ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
			ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
			ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
			ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
			ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + ptr_lamt[4] + ptr_lamt[5];
			ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + ptr_lam[5] + ptr_lamt[5]*ptr_db[5] + ptr_dlam[5] - ptr_lam[4] - ptr_lamt[4]*ptr_db[4] - ptr_dlam[4];
			ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

			ptr_tinv[6] = 1.0/ptr_t[6];
			ptr_tinv[7] = 1.0/ptr_t[7];
			ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
			ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
			ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
			ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
			ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + ptr_lamt[6] + ptr_lamt[7];
			ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + ptr_lam[7] + ptr_lamt[7]*ptr_db[7] + ptr_dlam[7] - ptr_lam[6] - ptr_lamt[6]*ptr_db[6] - ptr_dlam[6];
			ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

			ptr_t     += 8;
			ptr_lam   += 8;
			ptr_lamt  += 8;
			ptr_dlam  += 8;
			ptr_tinv  += 8;
			ptr_db    += 8;
			ptr_Z     += 8;
			ptr_z     += 8;
			ptr_Zl    += 8;
			ptr_zl    += 8;

			}
		if(ii<nbu)
			{
			// clean-up loop
			bs0 = nbu-ii;
			ll = 0;
			for(; ll<bs0; ll++)
				{
				ptr_tinv[0] = 1.0/ptr_t[0];
				ptr_tinv[1] = 1.0/ptr_t[1];
				ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
				ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
				ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
				ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
				ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + ptr_lamt[0] + ptr_lamt[1];
				ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + ptr_lam[1] + ptr_lamt[1]*ptr_db[1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[0] - ptr_dlam[0];
				ptr_pl2[ii+ll+0] = ptr_pl[(ii+ll)*bs];

				ptr_t     += 2;
				ptr_lam   += 2;
				ptr_lamt  += 2;
				ptr_dlam  += 2;
				ptr_tinv  += 2;
				ptr_db    += 2;
				ptr_Z     += 2;
				ptr_z     += 2;
				ptr_Zl    += 2;
				ptr_zl    += 2;
				}
			
			// soft constraints on x
			// clean-up loop
			bs0 = nb-ii<4 ? nb-ii : 4 ;
			for(; ll<bs0; ll++)
				{
				ptr_tinv[0] = 1.0/ptr_t[0];
				ptr_tinv[1] = 1.0/ptr_t[1];
				ptr_tinv[anb+0] = 1.0/ptr_t[anb+0];
				ptr_tinv[anb+1] = 1.0/ptr_t[anb+1];
				ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
				ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
				ptr_lamt[anb+0] = ptr_lam[anb+0]*ptr_tinv[anb+0];
				ptr_lamt[anb+1] = ptr_lam[anb+1]*ptr_tinv[anb+1];
				ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
				ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
				ptr_dlam[anb+0] = ptr_tinv[anb+0]*sigma_mu; // !!!!!
				ptr_dlam[anb+1] = ptr_tinv[anb+1]*sigma_mu; // !!!!!
				Qx[0] = ptr_lamt[0];
				Qx[1] = ptr_lamt[1];
				qx[0] = ptr_lam[0] + ptr_dlam[0] + ptr_lamt[0]*ptr_db[0];
				qx[1] = ptr_lam[1] + ptr_dlam[1] + ptr_lamt[1]*ptr_db[1];
				ptr_Zl[0] = 1.0 / (ptr_Z[0] + Qx[0] + ptr_lamt[anb+0]); // inverted of updated diagonal !!!
				ptr_Zl[1] = 1.0 / (ptr_Z[1] + Qx[1] + ptr_lamt[anb+1]); // inverted of updated diagonal !!!
				ptr_zl[0] = ptr_z[0] + qx[0] + ptr_lam[anb+0] + ptr_dlam[anb+0];
				ptr_zl[1] = ptr_z[1] + qx[1] + ptr_lam[anb+1] + ptr_dlam[anb+1];
				qx[0] = qx[0] - Qx[0]*ptr_zl[0]*ptr_Zl[0]; // update this before Qx !!!!!!!!!!!
				qx[1] = qx[1] - Qx[1]*ptr_zl[1]*ptr_Zl[1]; // update this before Qx !!!!!!!!!!!
				Qx[0] = Qx[0] - Qx[0]*Qx[0]*ptr_Zl[0];
				Qx[1] = Qx[1] - Qx[1]*Qx[1]*ptr_Zl[1];
				ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + Qx[1] + Qx[0];
				ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + qx[1] - qx[0];
				ptr_pl2[ii+ll] = ptr_pl[(ii+ll)*bs];

				ptr_t     += 2;
				ptr_lam   += 2;
				ptr_lamt  += 2;
				ptr_dlam  += 2;
				ptr_tinv  += 2;
				ptr_db    += 2;
				ptr_Z     += 2;
				ptr_z     += 2;
				ptr_Zl    += 2;
				ptr_zl    += 2;
				}
			ii += ll;
			}
		// main loop
		for(; ii<nb-3; ii+=4)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_tinv[anb+0] = 1.0/ptr_t[anb+0];
			ptr_tinv[anb+1] = 1.0/ptr_t[anb+1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_lamt[anb+0] = ptr_lam[anb+0]*ptr_tinv[anb+0];
			ptr_lamt[anb+1] = ptr_lam[anb+1]*ptr_tinv[anb+1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_dlam[anb+0] = ptr_tinv[anb+0]*sigma_mu; // !!!!!
			ptr_dlam[anb+1] = ptr_tinv[anb+1]*sigma_mu; // !!!!!
			Qx[0] = ptr_lamt[0];
			Qx[1] = ptr_lamt[1];
			qx[0] = ptr_lam[0] + ptr_dlam[0] + ptr_lamt[0]*ptr_db[0];
			qx[1] = ptr_lam[1] + ptr_dlam[1] + ptr_lamt[1]*ptr_db[1];
			ptr_Zl[0] = 1.0 / (ptr_Z[0] + Qx[0] + ptr_lamt[anb+0]); // inverted of updated diagonal !!!
			ptr_Zl[1] = 1.0 / (ptr_Z[1] + Qx[1] + ptr_lamt[anb+1]); // inverted of updated diagonal !!!
			ptr_zl[0] = ptr_z[0] + qx[0] + ptr_lam[anb+0] + ptr_dlam[anb+0];
			ptr_zl[1] = ptr_z[1] + qx[1] + ptr_lam[anb+1] + ptr_dlam[anb+1];
			qx[0] = qx[0] - Qx[0]*ptr_zl[0]*ptr_Zl[0]; // update this before Qx !!!!!!!!!!!
			qx[1] = qx[1] - Qx[1]*ptr_zl[1]*ptr_Zl[1]; // update this before Qx !!!!!!!!!!!
			Qx[0] = Qx[0] - Qx[0]*Qx[0]*ptr_Zl[0];
			Qx[1] = Qx[1] - Qx[1]*Qx[1]*ptr_Zl[1];
			ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + Qx[1] + Qx[0];
			ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + qx[1] - qx[0];
			ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

			ptr_tinv[2] = 1.0/ptr_t[2];
			ptr_tinv[3] = 1.0/ptr_t[3];
			ptr_tinv[anb+2] = 1.0/ptr_t[anb+2];
			ptr_tinv[anb+3] = 1.0/ptr_t[anb+3];
			ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
			ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
			ptr_lamt[anb+2] = ptr_lam[anb+2]*ptr_tinv[anb+2];
			ptr_lamt[anb+3] = ptr_lam[anb+3]*ptr_tinv[anb+3];
			ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
			ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
			ptr_dlam[anb+2] = ptr_tinv[anb+2]*sigma_mu; // !!!!!
			ptr_dlam[anb+3] = ptr_tinv[anb+3]*sigma_mu; // !!!!!
			Qx[2] = ptr_lamt[2];
			Qx[3] = ptr_lamt[3];
			qx[2] = ptr_lam[2] + ptr_dlam[2] + ptr_lamt[2]*ptr_db[2];
			qx[3] = ptr_lam[3] + ptr_dlam[3] + ptr_lamt[3]*ptr_db[3];
			ptr_Zl[2] = 1.0 / (ptr_Z[2] + Qx[2] + ptr_lamt[anb+2]); // inverted of updated diagonal !!!
			ptr_Zl[3] = 1.0 / (ptr_Z[3] + Qx[3] + ptr_lamt[anb+3]); // inverted of updated diagonal !!!
			ptr_zl[2] = ptr_z[2] + qx[2] + ptr_lam[anb+2] + ptr_dlam[anb+2];
			ptr_zl[3] = ptr_z[3] + qx[3] + ptr_lam[anb+3] + ptr_dlam[anb+3];
			qx[2] = qx[2] - Qx[2]*ptr_zl[2]*ptr_Zl[2]; // update this before Qx !!!!!!!!!!!
			qx[3] = qx[3] - Qx[3]*ptr_zl[3]*ptr_Zl[3]; // update this before Qx !!!!!!!!!!!
			Qx[2] = Qx[2] - Qx[2]*Qx[2]*ptr_Zl[2];
			Qx[3] = Qx[3] - Qx[3]*Qx[3]*ptr_Zl[3];
			ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + Qx[3] + Qx[2];
			ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + qx[3] - qx[2];
			ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

			ptr_tinv[4] = 1.0/ptr_t[4];
			ptr_tinv[5] = 1.0/ptr_t[5];
			ptr_tinv[anb+4] = 1.0/ptr_t[anb+4];
			ptr_tinv[anb+5] = 1.0/ptr_t[anb+5];
			ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
			ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
			ptr_lamt[anb+4] = ptr_lam[anb+4]*ptr_tinv[anb+4];
			ptr_lamt[anb+5] = ptr_lam[anb+5]*ptr_tinv[anb+5];
			ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
			ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
			ptr_dlam[anb+4] = ptr_tinv[anb+4]*sigma_mu; // !!!!!
			ptr_dlam[anb+5] = ptr_tinv[anb+5]*sigma_mu; // !!!!!
			Qx[4] = ptr_lamt[4];
			Qx[5] = ptr_lamt[5];
			qx[4] = ptr_lam[4] + ptr_dlam[4] + ptr_lamt[4]*ptr_db[4];
			qx[5] = ptr_lam[5] + ptr_dlam[5] + ptr_lamt[5]*ptr_db[5];
			ptr_Zl[4] = 1.0 / (ptr_Z[4] + Qx[4] + ptr_lamt[anb+4]); // inverted of updated diagonal !!!
			ptr_Zl[5] = 1.0 / (ptr_Z[5] + Qx[5] + ptr_lamt[anb+5]); // inverted of updated diagonal !!!
			ptr_zl[4] = ptr_z[4] + qx[4] + ptr_lam[anb+4] + ptr_dlam[anb+4];
			ptr_zl[5] = ptr_z[5] + qx[5] + ptr_lam[anb+5] + ptr_dlam[anb+5];
			qx[4] = qx[4] - Qx[4]*ptr_zl[4]*ptr_Zl[4]; // update this before Qx !!!!!!!!!!!
			qx[5] = qx[5] - Qx[5]*ptr_zl[5]*ptr_Zl[5]; // update this before Qx !!!!!!!!!!!
			Qx[4] = Qx[4] - Qx[4]*Qx[4]*ptr_Zl[4];
			Qx[5] = Qx[5] - Qx[5]*Qx[5]*ptr_Zl[5];
			ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + Qx[5] + Qx[4];
			ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + qx[5] - qx[4];
			ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

			ptr_tinv[6] = 1.0/ptr_t[6];
			ptr_tinv[7] = 1.0/ptr_t[7];
			ptr_tinv[anb+6] = 1.0/ptr_t[anb+6];
			ptr_tinv[anb+7] = 1.0/ptr_t[anb+7];
			ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
			ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
			ptr_lamt[anb+6] = ptr_lam[anb+6]*ptr_tinv[anb+6];
			ptr_lamt[anb+7] = ptr_lam[anb+7]*ptr_tinv[anb+7];
			ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
			ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
			ptr_dlam[anb+6] = ptr_tinv[anb+6]*sigma_mu; // !!!!!
			ptr_dlam[anb+7] = ptr_tinv[anb+7]*sigma_mu; // !!!!!
			Qx[6] = ptr_lamt[6];
			Qx[7] = ptr_lamt[7];
			qx[6] = ptr_lam[6] + ptr_dlam[6] + ptr_lamt[6]*ptr_db[6];
			qx[7] = ptr_lam[7] + ptr_dlam[7] + ptr_lamt[7]*ptr_db[7];
			ptr_Zl[6] = 1.0 / (ptr_Z[6] + Qx[6] + ptr_lamt[anb+6]); // inverted of updated diagonal !!!
			ptr_Zl[7] = 1.0 / (ptr_Z[7] + Qx[7] + ptr_lamt[anb+7]); // inverted of updated diagonal !!!
			ptr_zl[6] = ptr_z[6] + qx[6] + ptr_lam[anb+6] + ptr_dlam[anb+6];
			ptr_zl[7] = ptr_z[7] + qx[7] + ptr_lam[anb+7] + ptr_dlam[anb+7];
			qx[6] = qx[6] - Qx[6]*ptr_zl[6]*ptr_Zl[6]; // update this before Qx !!!!!!!!!!!
			qx[7] = qx[7] - Qx[7]*ptr_zl[7]*ptr_Zl[7]; // update this before Qx !!!!!!!!!!!
			Qx[6] = Qx[6] - Qx[6]*Qx[6]*ptr_Zl[6];
			Qx[7] = Qx[7] - Qx[7]*Qx[7]*ptr_Zl[7];
			ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + Qx[7] + Qx[6];
			ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + qx[7] - qx[6];
			ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

			ptr_t     += 8;
			ptr_lam   += 8;
			ptr_lamt  += 8;
			ptr_dlam  += 8;
			ptr_tinv  += 8;
			ptr_db    += 8;
			ptr_Z     += 8;
			ptr_z     += 8;
			ptr_Zl    += 8;
			ptr_zl    += 8;

			}
		if(ii<nb)
			{
			bs0 = nb-ii;
			for(ll=0; ll<bs0; ll++)
				{
				ptr_tinv[0] = 1.0/ptr_t[0];
				ptr_tinv[1] = 1.0/ptr_t[1];
				ptr_tinv[anb+0] = 1.0/ptr_t[anb+0];
				ptr_tinv[anb+1] = 1.0/ptr_t[anb+1];
				ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
				ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
				ptr_lamt[anb+0] = ptr_lam[anb+0]*ptr_tinv[anb+0];
				ptr_lamt[anb+1] = ptr_lam[anb+1]*ptr_tinv[anb+1];
				ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
				ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
				ptr_dlam[anb+0] = ptr_tinv[anb+0]*sigma_mu; // !!!!!
				ptr_dlam[anb+1] = ptr_tinv[anb+1]*sigma_mu; // !!!!!
				Qx[0] = ptr_lamt[0];
				Qx[1] = ptr_lamt[1];
				qx[0] = ptr_lam[0] + ptr_dlam[0] + ptr_lamt[0]*ptr_db[0];
				qx[1] = ptr_lam[1] + ptr_dlam[1] + ptr_lamt[1]*ptr_db[1];
				ptr_Zl[0] = 1.0 / (ptr_Z[0] + Qx[0] + ptr_lamt[anb+0]); // inverted of updated diagonal !!!
				ptr_Zl[1] = 1.0 / (ptr_Z[1] + Qx[1] + ptr_lamt[anb+1]); // inverted of updated diagonal !!!
				ptr_zl[0] = ptr_z[0] + qx[0] + ptr_lam[anb+0] + ptr_dlam[anb+0];
				ptr_zl[1] = ptr_z[1] + qx[1] + ptr_lam[anb+1] + ptr_dlam[anb+1];
				qx[0] = qx[0] - Qx[0]*ptr_zl[0]*ptr_Zl[0]; // update this before Qx !!!!!!!!!!!
				qx[1] = qx[1] - Qx[1]*ptr_zl[1]*ptr_Zl[1]; // update this before Qx !!!!!!!!!!!
				Qx[0] = Qx[0] - Qx[0]*Qx[0]*ptr_Zl[0];
				Qx[1] = Qx[1] - Qx[1]*Qx[1]*ptr_Zl[1];
				ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + Qx[1] + Qx[0];
				ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + qx[1] - qx[0];
				ptr_pl2[ii+ll] = ptr_pl[(ii+ll)*bs];

				ptr_t     += 2;
				ptr_lam   += 2;
				ptr_lamt  += 2;
				ptr_dlam  += 2;
				ptr_tinv  += 2;
				ptr_db    += 2;
				ptr_Z     += 2;
				ptr_z     += 2;
				ptr_Zl    += 2;
				ptr_zl    += 2;
				}
			}
	
		}

	// last stage

	ptr_t     = t[N]     + 2*nu;
	ptr_lam   = lam[N]   + 2*nu;
	ptr_lamt  = lamt[N]  + 2*nu;
	ptr_dlam  = dlam[N]  + 2*nu;
	ptr_tinv  = t_inv[N] + 2*nu;
	ptr_db    = db[N]    + 2*nu;
	ptr_Z     = Z[N]     + 2*nu;
	ptr_z     = z[N]     + 2*nu;
	ptr_Zl    = Zl[N]    + 2*nu;
	ptr_zl    = zl[N]    + 2*nu;
	ptr_pd    = pd[N];
	ptr_pl    = pl[N];
	ptr_pl2   = pl2[N];
	ptr_bd    = bd[N];
	ptr_bl    = bl[N];

	ii=4*(nu/4); // k1 supposed to be multiple of bs !!!!!!!!!! NO MORE !!!!!!!
	if(ii<nu)
		{
		bs0 = nb-ii<4 ? nb-ii : 4 ;
		ll = nu-ii; //k0%4;
		for(; ll<bs0; ll++)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_tinv[anb+0] = 1.0/ptr_t[anb+0];
			ptr_tinv[anb+1] = 1.0/ptr_t[anb+1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_lamt[anb+0] = ptr_lam[anb+0]*ptr_tinv[anb+0];
			ptr_lamt[anb+1] = ptr_lam[anb+1]*ptr_tinv[anb+1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_dlam[anb+0] = ptr_tinv[anb+0]*sigma_mu; // !!!!!
			ptr_dlam[anb+1] = ptr_tinv[anb+1]*sigma_mu; // !!!!!
			Qx[0] = ptr_lamt[0];
			Qx[1] = ptr_lamt[1];
			qx[0] = ptr_lam[0] + ptr_dlam[0] + ptr_lamt[0]*ptr_db[0];
			qx[1] = ptr_lam[1] + ptr_dlam[1] + ptr_lamt[1]*ptr_db[1];
			ptr_Zl[0] = 1.0 / (ptr_Z[0] + Qx[0] + ptr_lamt[anb+0]); // inverted of updated diagonal !!!
			ptr_Zl[1] = 1.0 / (ptr_Z[1] + Qx[1] + ptr_lamt[anb+1]); // inverted of updated diagonal !!!
			ptr_zl[0] = ptr_z[0] + qx[0] + ptr_lam[anb+0] + ptr_dlam[anb+0];
			ptr_zl[1] = ptr_z[1] + qx[1] + ptr_lam[anb+1] + ptr_dlam[anb+1];
			qx[0] = qx[0] - Qx[0]*ptr_zl[0]*ptr_Zl[0]; // update this before Qx !!!!!!!!!!!
			qx[1] = qx[1] - Qx[1]*ptr_zl[1]*ptr_Zl[1]; // update this before Qx !!!!!!!!!!!
			Qx[0] = Qx[0] - Qx[0]*Qx[0]*ptr_Zl[0];
			Qx[1] = Qx[1] - Qx[1]*Qx[1]*ptr_Zl[1];
			ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + Qx[1] + Qx[0];
			ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + qx[1] - qx[0];
			ptr_pl2[ii+ll] = ptr_pl[(ii+ll)*bs];

			ptr_t     += 2;
			ptr_lam   += 2;
			ptr_lamt  += 2;
			ptr_dlam  += 2;
			ptr_tinv  += 2;
			ptr_db    += 2;
			ptr_Z     += 2;
			ptr_z     += 2;
			ptr_Zl    += 2;
			ptr_zl    += 2;
			}
		ii += 4;
		}

	for(; ii<nb-3; ii+=4)
		{
		ptr_tinv[0] = 1.0/ptr_t[0];
		ptr_tinv[1] = 1.0/ptr_t[1];
		ptr_tinv[anb+0] = 1.0/ptr_t[anb+0];
		ptr_tinv[anb+1] = 1.0/ptr_t[anb+1];
		ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
		ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
		ptr_lamt[anb+0] = ptr_lam[anb+0]*ptr_tinv[anb+0];
		ptr_lamt[anb+1] = ptr_lam[anb+1]*ptr_tinv[anb+1];
		ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
		ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
		ptr_dlam[anb+0] = ptr_tinv[anb+0]*sigma_mu; // !!!!!
		ptr_dlam[anb+1] = ptr_tinv[anb+1]*sigma_mu; // !!!!!
		Qx[0] = ptr_lamt[0];
		Qx[1] = ptr_lamt[1];
		qx[0] = ptr_lam[0] + ptr_dlam[0] + ptr_lamt[0]*ptr_db[0];
		qx[1] = ptr_lam[1] + ptr_dlam[1] + ptr_lamt[1]*ptr_db[1];
		ptr_Zl[0] = 1.0 / (ptr_Z[0] + Qx[0] + ptr_lamt[anb+0]); // inverted of updated diagonal !!!
		ptr_Zl[1] = 1.0 / (ptr_Z[1] + Qx[1] + ptr_lamt[anb+1]); // inverted of updated diagonal !!!
		ptr_zl[0] = ptr_z[0] + qx[0] + ptr_lam[anb+0] + ptr_dlam[anb+0];
		ptr_zl[1] = ptr_z[1] + qx[1] + ptr_lam[anb+1] + ptr_dlam[anb+1];
		qx[0] = qx[0] - Qx[0]*ptr_zl[0]*ptr_Zl[0]; // update this before Qx !!!!!!!!!!!
		qx[1] = qx[1] - Qx[1]*ptr_zl[1]*ptr_Zl[1]; // update this before Qx !!!!!!!!!!!
		Qx[0] = Qx[0] - Qx[0]*Qx[0]*ptr_Zl[0];
		Qx[1] = Qx[1] - Qx[1]*Qx[1]*ptr_Zl[1];
		ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + Qx[1] + Qx[0];
		ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + qx[1] - qx[0];
		ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

		ptr_tinv[2] = 1.0/ptr_t[2];
		ptr_tinv[3] = 1.0/ptr_t[3];
		ptr_tinv[anb+2] = 1.0/ptr_t[anb+2];
		ptr_tinv[anb+3] = 1.0/ptr_t[anb+3];
		ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
		ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
		ptr_lamt[anb+2] = ptr_lam[anb+2]*ptr_tinv[anb+2];
		ptr_lamt[anb+3] = ptr_lam[anb+3]*ptr_tinv[anb+3];
		ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
		ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
		ptr_dlam[anb+2] = ptr_tinv[anb+2]*sigma_mu; // !!!!!
		ptr_dlam[anb+3] = ptr_tinv[anb+3]*sigma_mu; // !!!!!
		Qx[2] = ptr_lamt[2];
		Qx[3] = ptr_lamt[3];
		qx[2] = ptr_lam[2] + ptr_dlam[2] + ptr_lamt[2]*ptr_db[2];
		qx[3] = ptr_lam[3] + ptr_dlam[3] + ptr_lamt[3]*ptr_db[3];
		ptr_Zl[2] = 1.0 / (ptr_Z[2] + Qx[2] + ptr_lamt[anb+2]); // inverted of updated diagonal !!!
		ptr_Zl[3] = 1.0 / (ptr_Z[3] + Qx[3] + ptr_lamt[anb+3]); // inverted of updated diagonal !!!
		ptr_zl[2] = ptr_z[2] + qx[2] + ptr_lam[anb+2] + ptr_dlam[anb+2];
		ptr_zl[3] = ptr_z[3] + qx[3] + ptr_lam[anb+3] + ptr_dlam[anb+3];
		qx[2] = qx[2] - Qx[2]*ptr_zl[2]*ptr_Zl[2]; // update this before Qx !!!!!!!!!!!
		qx[3] = qx[3] - Qx[3]*ptr_zl[3]*ptr_Zl[3]; // update this before Qx !!!!!!!!!!!
		Qx[2] = Qx[2] - Qx[2]*Qx[2]*ptr_Zl[2];
		Qx[3] = Qx[3] - Qx[3]*Qx[3]*ptr_Zl[3];
		ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + Qx[3] + Qx[2];
		ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + qx[3] - qx[2];
		ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

		ptr_tinv[4] = 1.0/ptr_t[4];
		ptr_tinv[5] = 1.0/ptr_t[5];
		ptr_tinv[anb+4] = 1.0/ptr_t[anb+4];
		ptr_tinv[anb+5] = 1.0/ptr_t[anb+5];
		ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
		ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
		ptr_lamt[anb+4] = ptr_lam[anb+4]*ptr_tinv[anb+4];
		ptr_lamt[anb+5] = ptr_lam[anb+5]*ptr_tinv[anb+5];
		ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
		ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
		ptr_dlam[anb+4] = ptr_tinv[anb+4]*sigma_mu; // !!!!!
		ptr_dlam[anb+5] = ptr_tinv[anb+5]*sigma_mu; // !!!!!
		Qx[4] = ptr_lamt[4];
		Qx[5] = ptr_lamt[5];
		qx[4] = ptr_lam[4] + ptr_dlam[4] + ptr_lamt[4]*ptr_db[4];
		qx[5] = ptr_lam[5] + ptr_dlam[5] + ptr_lamt[5]*ptr_db[5];
		ptr_Zl[4] = 1.0 / (ptr_Z[4] + Qx[4] + ptr_lamt[anb+4]); // inverted of updated diagonal !!!
		ptr_Zl[5] = 1.0 / (ptr_Z[5] + Qx[5] + ptr_lamt[anb+5]); // inverted of updated diagonal !!!
		ptr_zl[4] = ptr_z[4] + qx[4] + ptr_lam[anb+4] + ptr_dlam[anb+4];
		ptr_zl[5] = ptr_z[5] + qx[5] + ptr_lam[anb+5] + ptr_dlam[anb+5];
		qx[4] = qx[4] - Qx[4]*ptr_zl[4]*ptr_Zl[4]; // update this before Qx !!!!!!!!!!!
		qx[5] = qx[5] - Qx[5]*ptr_zl[5]*ptr_Zl[5]; // update this before Qx !!!!!!!!!!!
		Qx[4] = Qx[4] - Qx[4]*Qx[4]*ptr_Zl[4];
		Qx[5] = Qx[5] - Qx[5]*Qx[5]*ptr_Zl[5];
		ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + Qx[5] + Qx[4];
		ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + qx[5] - qx[4];
		ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

		ptr_tinv[6] = 1.0/ptr_t[6];
		ptr_tinv[7] = 1.0/ptr_t[7];
		ptr_tinv[anb+6] = 1.0/ptr_t[anb+6];
		ptr_tinv[anb+7] = 1.0/ptr_t[anb+7];
		ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
		ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
		ptr_lamt[anb+6] = ptr_lam[anb+6]*ptr_tinv[anb+6];
		ptr_lamt[anb+7] = ptr_lam[anb+7]*ptr_tinv[anb+7];
		ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
		ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
		ptr_dlam[anb+6] = ptr_tinv[anb+6]*sigma_mu; // !!!!!
		ptr_dlam[anb+7] = ptr_tinv[anb+7]*sigma_mu; // !!!!!
		Qx[6] = ptr_lamt[6];
		Qx[7] = ptr_lamt[7];
		qx[6] = ptr_lam[6] + ptr_dlam[6] + ptr_lamt[6]*ptr_db[6];
		qx[7] = ptr_lam[7] + ptr_dlam[7] + ptr_lamt[7]*ptr_db[7];
		ptr_Zl[6] = 1.0 / (ptr_Z[6] + Qx[6] + ptr_lamt[anb+6]); // inverted of updated diagonal !!!
		ptr_Zl[7] = 1.0 / (ptr_Z[7] + Qx[7] + ptr_lamt[anb+7]); // inverted of updated diagonal !!!
		ptr_zl[6] = ptr_z[6] + qx[6] + ptr_lam[anb+6] + ptr_dlam[anb+6];
		ptr_zl[7] = ptr_z[7] + qx[7] + ptr_lam[anb+7] + ptr_dlam[anb+7];
		qx[6] = qx[6] - Qx[6]*ptr_zl[6]*ptr_Zl[6]; // update this before Qx !!!!!!!!!!!
		qx[7] = qx[7] - Qx[7]*ptr_zl[7]*ptr_Zl[7]; // update this before Qx !!!!!!!!!!!
		Qx[6] = Qx[6] - Qx[6]*Qx[6]*ptr_Zl[6];
		Qx[7] = Qx[7] - Qx[7]*Qx[7]*ptr_Zl[7];
		ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + Qx[7] + Qx[6];
		ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + qx[7] - qx[6];
		ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

		ptr_t     += 8;
		ptr_lam   += 8;
		ptr_lamt  += 8;
		ptr_dlam  += 8;
		ptr_tinv  += 8;
		ptr_db    += 8;
		ptr_Z     += 8;
		ptr_z     += 8;
		ptr_Zl    += 8;
		ptr_zl    += 8;

		}
	if(ii<nb)
		{
		bs0 = nb-ii;
		for(ll=0; ll<bs0; ll++)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_tinv[anb+0] = 1.0/ptr_t[anb+0];
			ptr_tinv[anb+1] = 1.0/ptr_t[anb+1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_lamt[anb+0] = ptr_lam[anb+0]*ptr_tinv[anb+0];
			ptr_lamt[anb+1] = ptr_lam[anb+1]*ptr_tinv[anb+1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_dlam[anb+0] = ptr_tinv[anb+0]*sigma_mu; // !!!!!
			ptr_dlam[anb+1] = ptr_tinv[anb+1]*sigma_mu; // !!!!!
			Qx[0] = ptr_lamt[0];
			Qx[1] = ptr_lamt[1];
			qx[0] = ptr_lam[0] + ptr_dlam[0] + ptr_lamt[0]*ptr_db[0];
			qx[1] = ptr_lam[1] + ptr_dlam[1] + ptr_lamt[1]*ptr_db[1];
			ptr_Zl[0] = 1.0 / (ptr_Z[0] + Qx[0] + ptr_lamt[anb+0]); // inverted of updated diagonal !!!
			ptr_Zl[1] = 1.0 / (ptr_Z[1] + Qx[1] + ptr_lamt[anb+1]); // inverted of updated diagonal !!!
			ptr_zl[0] = ptr_z[0] + qx[0] + ptr_lam[anb+0] + ptr_dlam[anb+0];
			ptr_zl[1] = ptr_z[1] + qx[1] + ptr_lam[anb+1] + ptr_dlam[anb+1];
			qx[0] = qx[0] - Qx[0]*ptr_zl[0]*ptr_Zl[0]; // update this before Qx !!!!!!!!!!!
			qx[1] = qx[1] - Qx[1]*ptr_zl[1]*ptr_Zl[1]; // update this before Qx !!!!!!!!!!!
			Qx[0] = Qx[0] - Qx[0]*Qx[0]*ptr_Zl[0];
			Qx[1] = Qx[1] - Qx[1]*Qx[1]*ptr_Zl[1];
			ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + Qx[1] + Qx[0];
			ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + qx[1] - qx[0];
			ptr_pl2[ii+ll] = ptr_pl[(ii+ll)*bs];

			ptr_t     += 2;
			ptr_lam   += 2;
			ptr_lamt  += 2;
			ptr_dlam  += 2;
			ptr_tinv  += 2;
			ptr_db    += 2;
			ptr_Z     += 2;
			ptr_z     += 2;
			ptr_Zl    += 2;
			ptr_zl    += 2;
			}
		}


	}



void d_compute_alpha_box_mpc(int N, int nx, int nu, int nb, double *ptr_alpha, double **t, double **dt, double **lam, double **dlam, double **lamt, double **dux, double **db)
	{
	
/*	const int bs = 4; //d_get_mr();*/

	const int nbu = nu<nb ? nu : nb ;
	
	double alpha = ptr_alpha[0];
	
/*	int kna = ((k1+bs-1)/bs)*bs;*/

	int jj, ll;


	// first stage

	ll = 0;
	for(; ll<nbu; ll++)
		{

		dt[0][2*ll+0] =   dux[0][ll] - db[0][2*ll+0] - t[0][2*ll+0];
		dt[0][2*ll+1] = - dux[0][ll] - db[0][2*ll+1] - t[0][2*ll+1];
		dlam[0][2*ll+0] -= lamt[0][2*ll+0] * dt[0][2*ll+0] + lam[0][2*ll+0];
		dlam[0][2*ll+1] -= lamt[0][2*ll+1] * dt[0][2*ll+1] + lam[0][2*ll+1];
		if( -alpha*dlam[0][2*ll+0]>lam[0][2*ll+0] )
			{
			alpha = - lam[0][2*ll+0] / dlam[0][2*ll+0];
			}
		if( -alpha*dlam[0][2*ll+1]>lam[0][2*ll+1] )
			{
			alpha = - lam[0][2*ll+1] / dlam[0][2*ll+1];
			}
		if( -alpha*dt[0][2*ll+0]>t[0][2*ll+0] )
			{
			alpha = - t[0][2*ll+0] / dt[0][2*ll+0];
			}
		if( -alpha*dt[0][2*ll+1]>t[0][2*ll+1] )
			{
			alpha = - t[0][2*ll+1] / dt[0][2*ll+1];
			}

		}

	// middle stages
	for(jj=1; jj<N; jj++)
		{

		ll = 0;
		for(; ll<nb; ll++)
			{

			dt[jj][2*ll+0] =   dux[jj][ll] - db[jj][2*ll+0] - t[jj][2*ll+0];
			dt[jj][2*ll+1] = - dux[jj][ll] - db[jj][2*ll+1] - t[jj][2*ll+1];
			dlam[jj][2*ll+0] -= lamt[jj][2*ll+0] * dt[jj][2*ll+0] + lam[jj][2*ll+0];
			dlam[jj][2*ll+1] -= lamt[jj][2*ll+1] * dt[jj][2*ll+1] + lam[jj][2*ll+1];
			if( -alpha*dlam[jj][2*ll+0]>lam[jj][2*ll+0] )
				{
				alpha = - lam[jj][2*ll+0] / dlam[jj][2*ll+0];
				}
			if( -alpha*dlam[jj][2*ll+1]>lam[jj][2*ll+1] )
				{
				alpha = - lam[jj][2*ll+1] / dlam[jj][2*ll+1];
				}
			if( -alpha*dt[jj][2*ll+0]>t[jj][2*ll+0] )
				{
				alpha = - t[jj][2*ll+0] / dt[jj][2*ll+0];
				}
			if( -alpha*dt[jj][2*ll+1]>t[jj][2*ll+1] )
				{
				alpha = - t[jj][2*ll+1] / dt[jj][2*ll+1];
				}

			}

		}		

	// last stage
	ll = nu;
	for(; ll<nb; ll++)
		{

		dt[N][2*ll+0] =   dux[N][ll] - db[N][2*ll+0] - t[N][2*ll+0];
		dt[N][2*ll+1] = - dux[N][ll] - db[N][2*ll+1] - t[N][2*ll+1];
		dlam[N][2*ll+0] -= lamt[N][2*ll+0] * dt[N][2*ll+0] + lam[N][2*ll+0];
		dlam[N][2*ll+1] -= lamt[N][2*ll+1] * dt[N][2*ll+1] + lam[N][2*ll+1];
		if( -alpha*dlam[N][2*ll+0]>lam[N][2*ll+0] )
			{
			alpha = - lam[N][2*ll+0] / dlam[N][2*ll+0];
			}
		if( -alpha*dlam[N][2*ll+1]>lam[N][2*ll+1] )
			{
			alpha = - lam[N][2*ll+1] / dlam[N][2*ll+1];
			}
		if( -alpha*dt[N][2*ll+0]>t[N][2*ll+0] )
			{
			alpha = - t[N][2*ll+0] / dt[N][2*ll+0];
			}
		if( -alpha*dt[N][2*ll+1]>t[N][2*ll+1] )
			{
			alpha = - t[N][2*ll+1] / dt[N][2*ll+1];
			}

		}
	
	ptr_alpha[0] = alpha;

	return;
	
	}



void d_compute_alpha_soft_mpc(int N, int nx, int nu, int nb, double *ptr_alpha, double **t, double **dt, double **lam, double **dlam, double **lamt, double **dux, double **db, double **Zl, double **zl)
	{
	
/*	const int bs = 4; //d_get_mr();*/

	const int nbu = nu<nb ? nu : nb ;
	const int nbx = nb-nu>0 ? nb-nu : 0 ;

	// constants
	const int bs = 4; //D_MR;
	const int ncl = D_NCL;
	const int nal = bs*ncl; // number of doubles per cache line

	const int anb = nal*((2*nb+nal-1)/nal); // cache aligned number of box and soft constraints // !!!!! doubled to include soft constraints !!!!!

	
	double alpha = ptr_alpha[0];
	
/*	int kna = ((k1+bs-1)/bs)*bs;*/

	int jj, ll;


	// first stage

	ll = 0;
	// hard input constraints
	for(; ll<nbu; ll++)
		{

		dt[0][2*ll+0] =   dux[0][ll] - db[0][2*ll+0] - t[0][2*ll+0];
		dt[0][2*ll+1] = - dux[0][ll] - db[0][2*ll+1] - t[0][2*ll+1];
		dlam[0][2*ll+0] -= lamt[0][2*ll+0] * dt[0][2*ll+0] + lam[0][2*ll+0];
		dlam[0][2*ll+1] -= lamt[0][2*ll+1] * dt[0][2*ll+1] + lam[0][2*ll+1];
		if( -alpha*dlam[0][2*ll+0]>lam[0][2*ll+0] )
			{
			alpha = - lam[0][2*ll+0] / dlam[0][2*ll+0];
			}
		if( -alpha*dlam[0][2*ll+1]>lam[0][2*ll+1] )
			{
			alpha = - lam[0][2*ll+1] / dlam[0][2*ll+1];
			}
		if( -alpha*dt[0][2*ll+0]>t[0][2*ll+0] )
			{
			alpha = - t[0][2*ll+0] / dt[0][2*ll+0];
			}
		if( -alpha*dt[0][2*ll+1]>t[0][2*ll+1] )
			{
			alpha = - t[0][2*ll+1] / dt[0][2*ll+1];
			}

		}

	// middle stages
	for(jj=1; jj<N; jj++)
		{

		ll = 0;
		// hard input constraints
		for(; ll<nbu; ll++)
			{

			dt[jj][2*ll+0] =   dux[jj][ll] - db[jj][2*ll+0] - t[jj][2*ll+0];
			dt[jj][2*ll+1] = - dux[jj][ll] - db[jj][2*ll+1] - t[jj][2*ll+1];
			dlam[jj][2*ll+0] -= lamt[jj][2*ll+0] * dt[jj][2*ll+0] + lam[jj][2*ll+0];
			dlam[jj][2*ll+1] -= lamt[jj][2*ll+1] * dt[jj][2*ll+1] + lam[jj][2*ll+1];
			if( -alpha*dlam[jj][2*ll+0]>lam[jj][2*ll+0] )
				{
				alpha = - lam[jj][2*ll+0] / dlam[jj][2*ll+0];
				}
			if( -alpha*dlam[jj][2*ll+1]>lam[jj][2*ll+1] )
				{
				alpha = - lam[jj][2*ll+1] / dlam[jj][2*ll+1];
				}
			if( -alpha*dt[jj][2*ll+0]>t[jj][2*ll+0] )
				{
				alpha = - t[jj][2*ll+0] / dt[jj][2*ll+0];
				}
			if( -alpha*dt[jj][2*ll+1]>t[jj][2*ll+1] )
				{
				alpha = - t[jj][2*ll+1] / dt[jj][2*ll+1];
				}

			}
		// soft state constraints
		for(; ll<nb; ll++)
			{

			dt[jj][anb+2*ll+0] = ( zl[jj][2*ll+0] - lamt[jj][2*ll+0]*dux[jj][ll] ) * Zl[jj][2*ll+0];
			dt[jj][anb+2*ll+1] = ( zl[jj][2*ll+1] + lamt[jj][2*ll+1]*dux[jj][ll] ) * Zl[jj][2*ll+1];
			dt[jj][2*ll+0] = dt[jj][anb+2*ll+0] + dux[jj][ll] - db[jj][2*ll+0] - t[jj][2*ll+0];
			dt[jj][2*ll+1] = dt[jj][anb+2*ll+1] - dux[jj][ll] - db[jj][2*ll+1] - t[jj][2*ll+1];
			dt[jj][anb+2*ll+0] -= t[jj][anb+2*ll+0];
			dt[jj][anb+2*ll+1] -= t[jj][anb+2*ll+1];
			dlam[jj][2*ll+0] -= lamt[jj][2*ll+0] * dt[jj][2*ll+0] + lam[jj][2*ll+0];
			dlam[jj][2*ll+1] -= lamt[jj][2*ll+1] * dt[jj][2*ll+1] + lam[jj][2*ll+1];
			dlam[jj][anb+2*ll+0] -= lamt[jj][anb+2*ll+0] * dt[jj][anb+2*ll+0] + lam[jj][anb+2*ll+0];
			dlam[jj][anb+2*ll+1] -= lamt[jj][anb+2*ll+1] * dt[jj][anb+2*ll+1] + lam[jj][anb+2*ll+1];
			if( -alpha*dlam[jj][2*ll+0]>lam[jj][2*ll+0] )
				{
				alpha = - lam[jj][2*ll+0] / dlam[jj][2*ll+0];
				}
			if( -alpha*dlam[jj][2*ll+1]>lam[jj][2*ll+1] )
				{
				alpha = - lam[jj][2*ll+1] / dlam[jj][2*ll+1];
				}
			if( -alpha*dt[jj][2*ll+0]>t[jj][2*ll+0] )
				{
				alpha = - t[jj][2*ll+0] / dt[jj][2*ll+0];
				}
			if( -alpha*dt[jj][2*ll+1]>t[jj][2*ll+1] )
				{
				alpha = - t[jj][2*ll+1] / dt[jj][2*ll+1];
				}
			if( -alpha*dlam[jj][anb+2*ll+0]>lam[jj][anb+2*ll+0] )
				{
				alpha = - lam[jj][anb+2*ll+0] / dlam[jj][anb+2*ll+0];
				}
			if( -alpha*dlam[jj][anb+2*ll+1]>lam[jj][anb+2*ll+1] )
				{
				alpha = - lam[jj][anb+2*ll+1] / dlam[jj][anb+2*ll+1];
				}
			if( -alpha*dt[jj][anb+2*ll+0]>t[jj][anb+2*ll+0] )
				{
				alpha = - t[jj][anb+2*ll+0] / dt[jj][anb+2*ll+0];
				}
			if( -alpha*dt[jj][anb+2*ll+1]>t[jj][anb+2*ll+1] )
				{
				alpha = - t[jj][anb+2*ll+1] / dt[jj][anb+2*ll+1];
				}

			}

		}		

	// last stage
	ll = nu;
	for(; ll<nb; ll++)
		{

		dt[N][anb+2*ll+0] = ( zl[N][2*ll+0] - lamt[N][2*ll+0]*dux[N][ll] ) * Zl[N][2*ll+0];
		dt[N][anb+2*ll+1] = ( zl[N][2*ll+1] + lamt[N][2*ll+1]*dux[N][ll] ) * Zl[N][2*ll+1];
		dt[N][2*ll+0] = dt[N][anb+2*ll+0] + dux[N][ll] - db[N][2*ll+0] - t[N][2*ll+0];
		dt[N][2*ll+1] = dt[N][anb+2*ll+1] - dux[N][ll] - db[N][2*ll+1] - t[N][2*ll+1];
		dt[N][anb+2*ll+0] -= t[N][anb+2*ll+0];
		dt[N][anb+2*ll+1] -= t[N][anb+2*ll+1];
		dlam[N][2*ll+0] -= lamt[N][2*ll+0] * dt[N][2*ll+0] + lam[N][2*ll+0];
		dlam[N][2*ll+1] -= lamt[N][2*ll+1] * dt[N][2*ll+1] + lam[N][2*ll+1];
		dlam[N][anb+2*ll+0] -= lamt[N][anb+2*ll+0] * dt[N][anb+2*ll+0] + lam[N][anb+2*ll+0];
		dlam[N][anb+2*ll+1] -= lamt[N][anb+2*ll+1] * dt[N][anb+2*ll+1] + lam[N][anb+2*ll+1];
		if( -alpha*dlam[N][2*ll+0]>lam[N][2*ll+0] )
			{
			alpha = - lam[N][2*ll+0] / dlam[N][2*ll+0];
			}
		if( -alpha*dlam[N][2*ll+1]>lam[N][2*ll+1] )
			{
			alpha = - lam[N][2*ll+1] / dlam[N][2*ll+1];
			}
		if( -alpha*dt[N][2*ll+0]>t[N][2*ll+0] )
			{
			alpha = - t[N][2*ll+0] / dt[N][2*ll+0];
			}
		if( -alpha*dt[N][2*ll+1]>t[N][2*ll+1] )
			{
			alpha = - t[N][2*ll+1] / dt[N][2*ll+1];
			}
		if( -alpha*dlam[N][anb+2*ll+0]>lam[N][anb+2*ll+0] )
			{
			alpha = - lam[N][anb+2*ll+0] / dlam[N][anb+2*ll+0];
			}
		if( -alpha*dlam[N][anb+2*ll+1]>lam[N][anb+2*ll+1] )
			{
			alpha = - lam[N][anb+2*ll+1] / dlam[N][anb+2*ll+1];
			}
		if( -alpha*dt[N][anb+2*ll+0]>t[N][anb+2*ll+0] )
			{
			alpha = - t[N][anb+2*ll+0] / dt[N][anb+2*ll+0];
			}
		if( -alpha*dt[N][anb+2*ll+1]>t[N][anb+2*ll+1] )
			{
			alpha = - t[N][anb+2*ll+1] / dt[N][anb+2*ll+1];
			}


		}
	
	ptr_alpha[0] = alpha;

	return;
	
	}



void d_update_var_box_mpc(int N, int nx, int nu, int nb, double *ptr_mu, double mu_scal, double alpha, double **ux, double **dux, double **t, double **dt, double **lam, double **dlam, double **pi, double **dpi)
	{

	const int nbu = nu<nb ? nu : nb ;
	
	int jj, ll;
	
	double mu = 0;

	// update inputs
	for(ll=0; ll<nu; ll++)
		ux[0][ll] += alpha*(dux[0][ll] - ux[0][ll]);
	// box constraints
	for(ll=0; ll<2*nbu; ll+=2)
		{
		lam[0][ll+0] += alpha*dlam[0][ll+0];
		lam[0][ll+1] += alpha*dlam[0][ll+1];
		t[0][ll+0] += alpha*dt[0][ll+0];
		t[0][ll+1] += alpha*dt[0][ll+1];
		mu += lam[0][ll+0] * t[0][ll+0] + lam[0][ll+1] * t[0][ll+1];
		}

	for(jj=1; jj<N; jj++)
		{
		// update inputs
		for(ll=0; ll<nu; ll++)
			ux[jj][ll] += alpha*(dux[jj][ll] - ux[jj][ll]);
		// update states
		for(ll=0; ll<nx; ll++)
			ux[jj][nu+ll] += alpha*(dux[jj][nu+ll] - ux[jj][nu+ll]);
		// update equality constrained multipliers
		for(ll=0; ll<nx; ll++)
			pi[jj][ll] += alpha*(dpi[jj][ll] - pi[jj][ll]);
		// box constraints
		for(ll=0; ll<2*nb; ll+=2)
			{
			lam[jj][ll+0] += alpha*dlam[jj][ll+0];
			lam[jj][ll+1] += alpha*dlam[jj][ll+1];
			t[jj][ll+0] += alpha*dt[jj][ll+0];
			t[jj][ll+1] += alpha*dt[jj][ll+1];
			mu += lam[jj][ll+0] * t[jj][ll+0] + lam[jj][ll+1] * t[jj][ll+1];
			}
		}

	// update states
	for(ll=0; ll<nx; ll++)
		ux[N][nu+ll] += alpha*(dux[N][nu+ll] - ux[N][nu+ll]);
	// update equality constrained multipliers
	for(ll=0; ll<nx; ll++)
		pi[N][ll] += alpha*(dpi[N][ll] - pi[N][ll]);
	// box constraints
	for(ll=2*nu; ll<2*nb; ll+=2)
		{
		lam[N][ll+0] += alpha*dlam[N][ll+0];
		lam[N][ll+1] += alpha*dlam[N][ll+1];
		t[N][ll+0] += alpha*dt[N][ll+0];
		t[N][ll+1] += alpha*dt[N][ll+1];
		mu += lam[N][ll+0] * t[N][ll+0] + lam[N][ll+1] * t[N][ll+1];
		}
	mu *= mu_scal;

	ptr_mu[0] = mu;

	return;
	
	}



void d_update_var_soft_mpc(int N, int nx, int nu, int nb, double *ptr_mu, double mu_scal, double alpha, double **ux, double **dux, double **t, double **dt, double **lam, double **dlam, double **pi, double **dpi)
	{

	const int nbu = nu<nb ? nu : nb ;
	const int nbx = nb-nu>0 ? nb-nu : 0 ;

	// constants
	const int bs = 4; //D_MR;
	const int ncl = D_NCL;
	const int nal = bs*ncl; // number of doubles per cache line

	const int anb = nal*((2*nb+nal-1)/nal); // cache aligned number of box and soft constraints // !!!!! doubled to include soft constraints !!!!!

	int jj, ll;
	
	double mu = 0;

	// update inputs
	for(ll=0; ll<nu; ll++)
		ux[0][ll] += alpha*(dux[0][ll] - ux[0][ll]);
	// box constraints on inputs
	for(ll=0; ll<2*nbu; ll+=2)
		{
		lam[0][ll+0] += alpha*dlam[0][ll+0];
		lam[0][ll+1] += alpha*dlam[0][ll+1];
		t[0][ll+0] += alpha*dt[0][ll+0];
		t[0][ll+1] += alpha*dt[0][ll+1];
		mu += lam[0][ll+0] * t[0][ll+0] + lam[0][ll+1] * t[0][ll+1];
		}

	for(jj=1; jj<N; jj++)
		{
		// update inputs
		for(ll=0; ll<nu; ll++)
			ux[jj][ll] += alpha*(dux[jj][ll] - ux[jj][ll]);
		// update states
		for(ll=0; ll<nx; ll++)
			ux[jj][nu+ll] += alpha*(dux[jj][nu+ll] - ux[jj][nu+ll]);
		// update equality constrained multipliers
		for(ll=0; ll<nx; ll++)
			pi[jj][ll] += alpha*(dpi[jj][ll] - pi[jj][ll]);
		// box constraints on inputs
		ll = 0;
		for(; ll<2*nbu; ll+=2)
			{
			lam[jj][ll+0] += alpha*dlam[jj][ll+0];
			lam[jj][ll+1] += alpha*dlam[jj][ll+1];
			t[jj][ll+0] += alpha*dt[jj][ll+0];
			t[jj][ll+1] += alpha*dt[jj][ll+1];
			mu += lam[jj][ll+0] * t[jj][ll+0] + lam[jj][ll+1] * t[jj][ll+1];
			}
		// soft constraints on states
		for(; ll<2*nb; ll+=2)
			{
			lam[jj][ll+0] += alpha*dlam[jj][ll+0];
			lam[jj][ll+1] += alpha*dlam[jj][ll+1];
			t[jj][ll+0] += alpha*dt[jj][ll+0];
			t[jj][ll+1] += alpha*dt[jj][ll+1];
			lam[jj][anb+ll+0] += alpha*dlam[jj][anb+ll+0];
			lam[jj][anb+ll+1] += alpha*dlam[jj][anb+ll+1];
			t[jj][anb+ll+0] += alpha*dt[jj][anb+ll+0];
			t[jj][anb+ll+1] += alpha*dt[jj][anb+ll+1];
			mu += lam[jj][ll+0] * t[jj][ll+0] + lam[jj][ll+1] * t[jj][ll+1] + lam[jj][anb+ll+0] * t[jj][anb+ll+0] + lam[jj][anb+ll+1] * t[jj][anb+ll+1];
			}
		}

	// update states
	for(ll=0; ll<nx; ll++)
		ux[N][nu+ll] += alpha*(dux[N][nu+ll] - ux[N][nu+ll]);
	// update equality constrained multipliers
	for(ll=0; ll<nx; ll++)
		pi[N][ll] += alpha*(dpi[N][ll] - pi[N][ll]);
	// soft constraints on states
	for(ll=2*nu; ll<2*nb; ll+=2)
		{
		lam[N][ll+0] += alpha*dlam[N][ll+0];
		lam[N][ll+1] += alpha*dlam[N][ll+1];
		t[N][ll+0] += alpha*dt[N][ll+0];
		t[N][ll+1] += alpha*dt[N][ll+1];
		lam[N][anb+ll+0] += alpha*dlam[N][anb+ll+0];
		lam[N][anb+ll+1] += alpha*dlam[N][anb+ll+1];
		t[N][anb+ll+0] += alpha*dt[N][anb+ll+0];
		t[N][anb+ll+1] += alpha*dt[N][anb+ll+1];
		mu += lam[N][ll+0] * t[N][ll+0] + lam[N][ll+1] * t[N][ll+1] + lam[N][anb+ll+0] * t[N][anb+ll+0] + lam[N][anb+ll+1] * t[N][anb+ll+1];
		}
	mu *= mu_scal;

	ptr_mu[0] = mu;

	return;
	
	}



void d_compute_mu_box_mpc(int N, int nx, int nu, int nb, double *ptr_mu, double mu_scal, double alpha, double **lam, double **dlam, double **t, double **dt)
	{
	
	const int nbu = nu<nb ? nu : nb ;

	int jj, ll;
	
	double mu = 0;
	
	for(ll=0 ; ll<2*nbu; ll+=2)
		{
		mu += (lam[0][ll+0] + alpha*dlam[0][ll+0]) * (t[0][ll+0] + alpha*dt[0][ll+0]) + (lam[0][ll+1] + alpha*dlam[0][ll+1]) * (t[0][ll+1] + alpha*dt[0][ll+1]);
		}


	for(jj=1; jj<N; jj++)
		for(ll=0 ; ll<2*nb; ll+=2)
			mu += (lam[jj][ll+0] + alpha*dlam[jj][ll+0]) * (t[jj][ll+0] + alpha*dt[jj][ll+0]) + (lam[jj][ll+1] + alpha*dlam[jj][ll+1]) * (t[jj][ll+1] + alpha*dt[jj][ll+1]);

	for(ll=2*nu ; ll<2*nb; ll+=2)
		mu += (lam[N][ll+0] + alpha*dlam[N][ll+0]) * (t[N][ll+0] + alpha*dt[N][ll+0]) + (lam[N][ll+1] + alpha*dlam[N][ll+1]) * (t[N][ll+1] + alpha*dt[N][ll+1]);

	mu *= mu_scal;
		
	ptr_mu[0] = mu;

	return;

	}



void d_compute_mu_soft_mpc(int N, int nx, int nu, int nb, double *ptr_mu, double mu_scal, double alpha, double **lam, double **dlam, double **t, double **dt)
	{
	
	const int nbu = nu<nb ? nu : nb ;
	const int nbx = nb-nu>0 ? nb-nu : 0 ;

	// constants
	const int bs = D_MR;
	const int ncl = D_NCL;
	const int nal = bs*ncl; // number of doubles per cache line

	const int anb = nal*((2*nb+nal-1)/nal); // cache aligned number of box and soft constraints // !!!!! doubled to include soft constraints !!!!!

	int jj, ll;
	
	double mu = 0;
	
	// fist stage: bounds on u only
	for(ll=0; ll<2*nbu; ll+=2)
		{
		mu += (lam[0][ll+0] + alpha*dlam[0][ll+0]) * (t[0][ll+0] + alpha*dt[0][ll+0]) + (lam[0][ll+1] + alpha*dlam[0][ll+1]) * (t[0][ll+1] + alpha*dt[0][ll+1]);
		}

	// middle stages: bounds on both u and x
	for(jj=1; jj<N; jj++)
		{
		for(ll=0; ll<2*nb; ll+=2)
			mu += (lam[jj][ll+0] + alpha*dlam[jj][ll+0]) * (t[jj][ll+0] + alpha*dt[jj][ll+0]) + (lam[jj][ll+1] + alpha*dlam[jj][ll+1]) * (t[jj][ll+1] + alpha*dt[jj][ll+1]);
		for(ll=anb+2*nu; ll<anb+2*nb; ll+=2)
			mu += (lam[jj][ll+0] + alpha*dlam[jj][ll+0]) * (t[jj][ll+0] + alpha*dt[jj][ll+0]) + (lam[jj][ll+1] + alpha*dlam[jj][ll+1]) * (t[jj][ll+1] + alpha*dt[jj][ll+1]);
		}	

	// last stage: bounds on x only
	for(ll=2*nu; ll<2*nb; ll+=2)
		mu += (lam[N][ll+0] + alpha*dlam[N][ll+0]) * (t[N][ll+0] + alpha*dt[N][ll+0]) + (lam[N][ll+1] + alpha*dlam[N][ll+1]) * (t[N][ll+1] + alpha*dt[N][ll+1]);
	for(ll=anb+2*nu; ll<anb+2*nb; ll+=2)
		mu += (lam[N][ll+0] + alpha*dlam[N][ll+0]) * (t[N][ll+0] + alpha*dt[N][ll+0]) + (lam[N][ll+1] + alpha*dlam[N][ll+1]) * (t[N][ll+1] + alpha*dt[N][ll+1]);

	mu *= mu_scal;
		
	ptr_mu[0] = mu;

	return;

	}













void d_init_ux_pi_t_box_mhe_old(int N, int nx, int nu, int nbu, int nb, double **ux, double **pi, double **db, double **t, int warm_start)
	{
	
	int jj, ll, ii;
	
	double thr0 = 1e-3; // minimum distance from a constraint

	if(warm_start==1)
		{
		for(jj=0; jj<N; jj++)
			{
			for(ll=0; ll<2*nb; ll+=2)
				{
				t[jj][ll+0] = ux[jj][ll/2] - db[jj][ll+0];
				t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
				if(t[jj][ll+0] < thr0)
					{
					if(t[jj][ll+1] < thr0)
						{
						ux[jj][ll/2] = ( - db[jj][ll+1] + db[jj][ll+0])*0.5;
						t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
						t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
						}
					else
						{
						t[jj][ll+0] = thr0;
						ux[jj][ll/2] = db[jj][ll+0] + thr0;
						}
					}
				else if(t[jj][ll+1] < thr0)
					{
					t[jj][ll+1] = thr0;
					ux[jj][ll/2] = - db[jj][ll+1] - thr0;
					}
				}
			}
		for(ll=0; ll<2*nbu; ll++) // this has to be strictly positive !!!
			t[N][ll] = 1;
		for(ll=2*nu; ll<2*nb; ll+=2)
			{
			t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
			t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
			if(t[N][ll+0] < thr0)
				{
				if(t[N][ll+1] < thr0)
					{
					ux[N][ll/2] = ( - db[N][ll+1] + db[N][ll+0])*0.5;
					t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
					t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
					}
				else
					{
					t[N][ll+0] = thr0;
					ux[N][ll/2] = db[N][ll+0] + thr0;
					}
				}
			else if(t[N][ll+1] < thr0)
				{
				t[N][ll+1] = thr0;
				ux[N][ll/2] = - db[N][ll+1] - thr0;
				}
			}

		}
	else // cold start
		{
		for(jj=0; jj<N; jj++)
			{
			for(ll=0; ll<2*nb; ll+=2)
				{
				ux[jj][ll/2] = 0.0;
/*				t[jj][ll+0] = 1.0;*/
/*				t[jj][ll+1] = 1.0;*/
				t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
				t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
				if(t[jj][ll+0] < thr0)
					{
					if(t[jj][ll+1] < thr0)
						{
						ux[jj][ll/2] = ( - db[jj][ll+1] + db[jj][ll+0])*0.5;
						t[jj][ll+0] =   ux[jj][ll/2] - db[jj][ll+0];
						t[jj][ll+1] = - db[jj][ll+1] - ux[jj][ll/2];
						}
					else
						{
						t[jj][ll+0] = thr0;
						ux[jj][ll/2] = db[jj][ll+0] + thr0;
						}
					}
				else if(t[jj][ll+1] < thr0)
					{
					t[jj][ll+1] = thr0;
					ux[jj][ll/2] = - db[jj][ll+1] - thr0;
					}
				}
			for(ii=ll/2; ii<nx+nu; ii++)
				ux[jj][ii] = 0.0; // initialize remaining components of u and x to zero
			}
		for(ll=0; ll<2*nbu; ll++)
			t[N][ll] = 1.0; // this has to be strictly positive !!!
		for(ll=2*nu; ll<2*nb; ll+=2)
			{
			ux[N][ll/2] = 0.0;
/*			t[N][ll+0] = 1.0;*/
/*			t[N][ll+1] = 1.0;*/
			t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
			t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
			if(t[N][ll+0] < thr0)
				{
				if(t[N][ll+1] < thr0)
					{
					ux[N][ll/2] = ( - db[N][ll+1] + db[N][ll+0])*0.5;
					t[N][ll+0] =   ux[N][ll/2] - db[N][ll+0];
					t[N][ll+1] = - db[N][ll+1] - ux[N][ll/2];
					}
				else
					{
					t[N][ll+0] = thr0;
					ux[N][ll/2] = db[N][ll+0] + thr0;
					}
				}
			else if(t[N][ll+1] < thr0)
				{
				t[N][ll+1] = thr0;
				ux[N][ll/2] = - db[N][ll+1] - thr0;
				}
			}
		for(ii=ll/2; ii<nx+nu; ii++)
			ux[N][ii] = 0.0; // initialize remaining components of x to zero

		for(jj=0; jj<=N; jj++)
			for(ll=0; ll<nx; ll++)
				pi[jj][ll] = 0.0; // initialize multipliers to zero

		}
	
	}



void d_init_lam_mhe_old(int N, int nu, int nbu, int nb, double **t, double **lam)	// TODO approximate reciprocal
	{
	
	int jj, ll;
	
	for(jj=0; jj<N; jj++)
		{
		for(ll=0; ll<2*nb; ll++)
			lam[jj][ll] = 1/t[jj][ll];
/*			lam[jj][ll] = thr0/t[jj][ll];*/
		}
	for(ll=0; ll<2*nu; ll++)
		lam[N][ll] = 1.0; // this has to be strictly positive !!!
	for(ll=2*nu; ll<2*nb; ll++)
		lam[N][ll] = 1/t[jj][ll];
/*		lam[N][ll] = thr0/t[jj][ll];*/
	
	}



void d_update_hessian_box_mhe_old(int N, int k0_dummy, int k1, int kmax, int cnz, double sigma_mu, double **t, double **t_inv, double **lam, double **lamt, double **dlam, double **bd, double **bl, double **pd, double **pl, double **pl2, double **db)
	{
	
	const int bs = 4; //d_get_mr();
	
	double temp0, temp1;
	
	double *ptr_t, *ptr_lam, *ptr_lamt, *ptr_dlam, *ptr_tinv, *ptr_pd, *ptr_pl, *ptr_pl2, *ptr_bd, *ptr_bl, *ptr_db;
	
	int ii, jj, ll, bs0;
	
	// first & middle stages

	for(jj=0; jj<N; jj++)
		{
		
		ptr_t     = t[jj];
		ptr_lam   = lam[jj];
		ptr_lamt  = lamt[jj];
		ptr_dlam  = dlam[jj];
		ptr_tinv  = t_inv[jj];
		ptr_pd    = pd[jj];
		ptr_pl    = pl[jj];
		ptr_pl2   = pl2[jj];
		ptr_bd    = bd[jj];
		ptr_bl    = bl[jj];
		ptr_db    = db[jj];

		ii = 0;
		for(; ii<kmax-3; ii+=4)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + ptr_lamt[0] + ptr_lamt[1];
			ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + ptr_lam[1] + ptr_lamt[1]*ptr_db[2*ii+1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[2*ii+0] - ptr_dlam[0];
			ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

			ptr_tinv[2] = 1.0/ptr_t[2];
			ptr_tinv[3] = 1.0/ptr_t[3];
			ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
			ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
			ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
			ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
			ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + ptr_lamt[2] + ptr_lamt[3];
			ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + ptr_lam[3] + ptr_lamt[3]*ptr_db[2*ii+3] + ptr_dlam[3] - ptr_lam[2] - ptr_lamt[2]*ptr_db[2*ii+2] - ptr_dlam[2];
			ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

			ptr_tinv[4] = 1.0/ptr_t[4];
			ptr_tinv[5] = 1.0/ptr_t[5];
			ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
			ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
			ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
			ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
			ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + ptr_lamt[4] + ptr_lamt[5];
			ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + ptr_lam[5] + ptr_lamt[5]*ptr_db[2*ii+5] + ptr_dlam[5] - ptr_lam[4] - ptr_lamt[4]*ptr_db[2*ii+4] - ptr_dlam[4];
			ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

			ptr_tinv[6] = 1.0/ptr_t[6];
			ptr_tinv[7] = 1.0/ptr_t[7];
			ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
			ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
			ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
			ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
			ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + ptr_lamt[6] + ptr_lamt[7];
			ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + ptr_lam[7] + ptr_lamt[7]*ptr_db[2*ii+7] + ptr_dlam[7] - ptr_lam[6] - ptr_lamt[6]*ptr_db[2*ii+6] - ptr_dlam[6];
			ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

			ptr_t     += 8;
			ptr_lam   += 8;
			ptr_lamt  += 8;
			ptr_dlam  += 8;
			ptr_tinv  += 8;

			}
		if(ii<kmax)
			{
			bs0 = kmax-ii;
			for(ll=0; ll<bs0; ll++)
				{
				ptr_tinv[0] = 1.0/ptr_t[0];
				ptr_tinv[1] = 1.0/ptr_t[1];
				ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
				ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
				ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
				ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
				ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + ptr_lamt[0] + ptr_lamt[1];
				ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + ptr_lam[1] + ptr_lamt[1]*ptr_db[2*ii+2*ll+1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[2*ii+2*ll+0] - ptr_dlam[0];
				ptr_pl2[ii+ll+0] = ptr_pl[(ii+ll)*bs];

				ptr_t     += 2;
				ptr_lam   += 2;
				ptr_lamt  += 2;
				ptr_dlam  += 2;
				ptr_tinv  += 2;
				}
			}
	
		}

	// last stage

	ptr_t     = t[N]     + 2*k1;
	ptr_lam   = lam[N]   + 2*k1;
	ptr_lamt  = lamt[N]  + 2*k1;
	ptr_dlam  = dlam[N]  + 2*k1;
	ptr_tinv  = t_inv[N] + 2*k1;
	ptr_pd    = pd[N];
	ptr_pl    = pl[N];
	ptr_pl2   = pl2[N];
	ptr_bd    = bd[N];
	ptr_bl    = bl[N];
	ptr_db    = db[N];

	ii=k1; // k1 supposed to be multiple of bs !!!!!!!!!!

	for(; ii<kmax-3; ii+=4)
		{
		ptr_tinv[0] = 1.0/ptr_t[0];
		ptr_tinv[1] = 1.0/ptr_t[1];
		ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
		ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
		ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
		ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
		ptr_pd[0+(ii+0)*bs+ii*cnz] = ptr_bd[ii+0] + ptr_lamt[0] + ptr_lamt[1];
		ptr_pl[(ii+0)*bs] = ptr_bl[ii+0] + ptr_lam[1] + ptr_lamt[1]*ptr_db[2*ii+1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[2*ii+0] - ptr_dlam[0];
		ptr_pl2[ii+0] = ptr_pl[(ii+0)*bs];

		ptr_tinv[2] = 1.0/ptr_t[2];
		ptr_tinv[3] = 1.0/ptr_t[3];
		ptr_lamt[2] = ptr_lam[2]*ptr_tinv[2];
		ptr_lamt[3] = ptr_lam[3]*ptr_tinv[3];
		ptr_dlam[2] = ptr_tinv[2]*sigma_mu; // !!!!!
		ptr_dlam[3] = ptr_tinv[3]*sigma_mu; // !!!!!
		ptr_pd[1+(ii+1)*bs+ii*cnz] = ptr_bd[ii+1] + ptr_lamt[2] + ptr_lamt[3];
		ptr_pl[(ii+1)*bs] = ptr_bl[ii+1] + ptr_lam[3] + ptr_lamt[3]*ptr_db[2*ii+3] + ptr_dlam[3] - ptr_lam[2] - ptr_lamt[2]*ptr_db[2*ii+2] - ptr_dlam[2];
		ptr_pl2[ii+1] = ptr_pl[(ii+1)*bs];

		ptr_tinv[4] = 1.0/ptr_t[4];
		ptr_tinv[5] = 1.0/ptr_t[5];
		ptr_lamt[4] = ptr_lam[4]*ptr_tinv[4];
		ptr_lamt[5] = ptr_lam[5]*ptr_tinv[5];
		ptr_dlam[4] = ptr_tinv[4]*sigma_mu; // !!!!!
		ptr_dlam[5] = ptr_tinv[5]*sigma_mu; // !!!!!
		ptr_pd[2+(ii+2)*bs+ii*cnz] = ptr_bd[ii+2] + ptr_lamt[4] + ptr_lamt[5];
		ptr_pl[(ii+2)*bs] = ptr_bl[ii+2] + ptr_lam[5] + ptr_lamt[5]*ptr_db[2*ii+5] + ptr_dlam[5] - ptr_lam[4] - ptr_lamt[4]*ptr_db[2*ii+4] - ptr_dlam[4];
		ptr_pl2[ii+2] = ptr_pl[(ii+2)*bs];

		ptr_tinv[6] = 1.0/ptr_t[6];
		ptr_tinv[7] = 1.0/ptr_t[7];
		ptr_lamt[6] = ptr_lam[6]*ptr_tinv[6];
		ptr_lamt[7] = ptr_lam[7]*ptr_tinv[7];
		ptr_dlam[6] = ptr_tinv[6]*sigma_mu; // !!!!!
		ptr_dlam[7] = ptr_tinv[7]*sigma_mu; // !!!!!
		ptr_pd[3+(ii+3)*bs+ii*cnz] = ptr_bd[ii+3] + ptr_lamt[6] + ptr_lamt[7];
		ptr_pl[(ii+3)*bs] = ptr_bl[ii+3] + ptr_lam[7] + ptr_lamt[7]*ptr_db[2*ii+7] + ptr_dlam[7] - ptr_lam[6] - ptr_lamt[6]*ptr_db[2*ii+6] - ptr_dlam[6];
		ptr_pl2[ii+3] = ptr_pl[(ii+3)*bs];

		ptr_t     += 8;
		ptr_lam   += 8;
		ptr_lamt  += 8;
		ptr_dlam  += 8;
		ptr_tinv  += 8;

		}
	if(ii<kmax)
		{
		bs0 = kmax-ii;
		for(ll=0; ll<bs0; ll++)
			{
			ptr_tinv[0] = 1.0/ptr_t[0];
			ptr_tinv[1] = 1.0/ptr_t[1];
			ptr_lamt[0] = ptr_lam[0]*ptr_tinv[0];
			ptr_lamt[1] = ptr_lam[1]*ptr_tinv[1];
			ptr_dlam[0] = ptr_tinv[0]*sigma_mu; // !!!!!
			ptr_dlam[1] = ptr_tinv[1]*sigma_mu; // !!!!!
			ptr_pd[ll+(ii+ll)*bs+ii*cnz] = ptr_bd[ii+ll] + ptr_lamt[0] + ptr_lamt[1];
			ptr_pl[(ii+ll)*bs] = ptr_bl[ii+ll] + ptr_lam[1] + ptr_lamt[1]*ptr_db[2*ii+2*ll+1] + ptr_dlam[1] - ptr_lam[0] - ptr_lamt[0]*ptr_db[2*ii+2*ll+0] - ptr_dlam[0];
			ptr_pl2[ii+ll+0] = ptr_pl[(ii+ll)*bs];

			ptr_t     += 2;
			ptr_lam   += 2;
			ptr_lamt  += 2;
			ptr_dlam  += 2;
			ptr_tinv  += 2;
			}
		}


	}



void d_compute_alpha_box_mhe_old(int N, int k0_dummy, int k1, int kmax, double *ptr_alpha, double **t, double **dt, double **lam, double **dlam, double **lamt, double **dux, double **db)
	{
	
/*	const int bs = 4; //d_get_mr();*/
	
	double alpha = ptr_alpha[0];
	
/*	int kna = ((k1+bs-1)/bs)*bs;*/

	int jj, ll;


	// first & middle stages
	for(jj=0; jj<N; jj++)
		{

		ll = 0;
		for(; ll<kmax; ll+=2)
			{

			dt[jj][ll+0] =   dux[jj][ll/2] - db[jj][ll+0] - t[jj][ll+0];
			dt[jj][ll+1] = - dux[jj][ll/2] - db[jj][ll+1] - t[jj][ll+1];
			dlam[jj][ll+0] -= lamt[jj][ll+0] * dt[jj][ll+0] + lam[jj][ll+0];
			dlam[jj][ll+1] -= lamt[jj][ll+1] * dt[jj][ll+1] + lam[jj][ll+1];
			if( -alpha*dlam[jj][ll+0]>lam[jj][ll+0] )
				{
				alpha = - lam[jj][ll+0] / dlam[jj][ll+0];
				}
			if( -alpha*dlam[jj][ll+1]>lam[jj][ll+1] )
				{
				alpha = - lam[jj][ll+1] / dlam[jj][ll+1];
				}
			if( -alpha*dt[jj][ll+0]>t[jj][ll+0] )
				{
				alpha = - t[jj][ll+0] / dt[jj][ll+0];
				}
			if( -alpha*dt[jj][ll+1]>t[jj][ll+1] )
				{
				alpha = - t[jj][ll+1] / dt[jj][ll+1];
				}

			}

		}		

	// last stage
	ll = k1;
	for(; ll<kmax; ll+=2)
		{

		dt[N][ll+0] =   dux[N][ll/2] - db[N][ll+0] - t[N][ll+0];
		dt[N][ll+1] = - dux[N][ll/2] - db[N][ll+1] - t[N][ll+1];
		dlam[N][ll+0] -= lamt[N][ll+0] * dt[N][ll+0] + lam[N][ll+0];
		dlam[N][ll+1] -= lamt[N][ll+1] * dt[N][ll+1] + lam[N][ll+1];
		if( -alpha*dlam[N][ll+0]>lam[N][ll+0] )
			{
			alpha = - lam[N][ll+0] / dlam[N][ll+0];
			}
		if( -alpha*dlam[N][ll+1]>lam[N][ll+1] )
			{
			alpha = - lam[N][ll+1] / dlam[N][ll+1];
			}
		if( -alpha*dt[N][ll+0]>t[N][ll+0] )
			{
			alpha = - t[N][ll+0] / dt[N][ll+0];
			}
		if( -alpha*dt[N][ll+1]>t[N][ll+1] )
			{
			alpha = - t[N][ll+1] / dt[N][ll+1];
			}

		}
	
	ptr_alpha[0] = alpha;

	return;
	
	}



void d_update_var_mhe_old(int nx, int nu, int N, int nb, int nbu, double *ptr_mu, double mu_scal, double alpha, double **ux, double **dux, double **t, double **dt, double **lam, double **dlam, double **pi, double **dpi)
	{
	
	int jj, ll;
	
	double mu = 0;

	for(jj=0; jj<N; jj++)
		{
		// update inputs
		for(ll=0; ll<nu; ll++)
			ux[jj][ll] += alpha*(dux[jj][ll] - ux[jj][ll]);
		// update states
		for(ll=0; ll<nx; ll++)
			ux[jj][nu+ll] += alpha*(dux[jj][nu+ll] - ux[jj][nu+ll]);
		// update equality constrained multipliers
		for(ll=0; ll<nx; ll++)
			pi[jj][ll] += alpha*(dpi[jj][ll] - pi[jj][ll]);
		// box constraints
		for(ll=0; ll<2*nb; ll+=2)
			{
			lam[jj][ll+0] += alpha*dlam[jj][ll+0];
			lam[jj][ll+1] += alpha*dlam[jj][ll+1];
			t[jj][ll+0] += alpha*dt[jj][ll+0];
			t[jj][ll+1] += alpha*dt[jj][ll+1];
			mu += lam[jj][ll+0] * t[jj][ll+0] + lam[jj][ll+1] * t[jj][ll+1];
			}
		}

	// update states
	for(ll=0; ll<nx; ll++)
		ux[N][nu+ll] += alpha*(dux[N][nu+ll] - ux[N][nu+ll]);
	// update equality constrained multipliers
	for(ll=0; ll<nx; ll++)
		pi[N][ll] += alpha*(dpi[N][ll] - pi[N][ll]);
	// box constraints
	for(ll=2*nu; ll<2*nb; ll+=2)
		{
		lam[N][ll+0] += alpha*dlam[N][ll+0];
		lam[N][ll+1] += alpha*dlam[N][ll+1];
		t[N][ll+0] += alpha*dt[N][ll+0];
		t[N][ll+1] += alpha*dt[N][ll+1];
		mu += lam[N][ll+0] * t[N][ll+0] + lam[N][ll+1] * t[N][ll+1];
		}
	mu *= mu_scal;

	ptr_mu[0] = mu;

	return;
	
	}



void d_compute_mu_mhe_old(int N, int nbu, int nu, int nb, double *ptr_mu, double mu_scal, double alpha, double **lam, double **dlam, double **t, double **dt)
	{
	
	int jj, ll;
	
	double mu = 0;
	
	for(jj=0; jj<N; jj++)
		for(ll=0 ; ll<2*nb; ll+=2)
			mu += (lam[jj][ll+0] + alpha*dlam[jj][ll+0]) * (t[jj][ll+0] + alpha*dt[jj][ll+0]) + (lam[jj][ll+1] + alpha*dlam[jj][ll+1]) * (t[jj][ll+1] + alpha*dt[jj][ll+1]);

	for(ll=2*nu ; ll<2*nb; ll+=2)
		mu += (lam[N][ll+0] + alpha*dlam[N][ll+0]) * (t[N][ll+0] + alpha*dt[N][ll+0]) + (lam[N][ll+1] + alpha*dlam[N][ll+1]) * (t[N][ll+1] + alpha*dt[N][ll+1]);

	mu *= mu_scal;
		
	ptr_mu[0] = mu;

	return;

	}

