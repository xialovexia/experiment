// Boneh-Lynn-Shacham short signatures demo.
//
// See the PBC_sig library for a practical implementation.
//
// Ben Lynn
#include "/usr/local/include/pbc/pbc.h"
#include "/usr/local/include/pbc/pbc_test.h"
#include <gmp.h>
#include stdlib.h;
#define NUM 100000

long long jiecheng(int n) {
	if (n == 1 || n==0) return 1;
	else return n*jiecheng(n - 1);
}

unsigned long cx(int n, int r) {
	if (r == 0) return 1;
	else if (n == 0||(n<r)) return 0;
	else return jiecheng(n) / (jiecheng(r)*jiecheng(n - r));
}


int main(int argc, char **argv) {
  //定义变量
  pairing_t pairing;
  gmp_randstate_t state;
  mpz_t tk;
  mpz_t x[NUM+1];
  element_t s[19];
  mpz_t bb1,r[19];
  element_t v[19];
  element_t sig[19];
  mpz_t total1[NUM+1];
  element_t total2[NUM+1];
  element_t g,h;
  element_t temp,temp1,temp3,temp4;
  element_t bb2,bb3;
  mpz_t qq,temp2;
  element_t qq1,ans2;
  double t0,t1,time;
  int i,j,l,y[19],d=19,k=9;
  unsigned long ans;
  int d1[NUM];
  for (i=0;i<NUM;i++)
  {
    d1[i]=rand();
  }

  pbc_demo_pairing_init(pairing, argc, argv);//在pbc_test.h中定义也是以文件流的方式读入参数
  for(i=0;i<19;i++){
     element_init_Zr(s[i], pairing);
     mpz_init(r[i]);  
     element_init_G1(v[i], pairing);
     element_init_G1(sig[i], pairing);
  }
  for(i=0;i<=NUM;i++){
     mpz_init(x[i]);
     mpz_init(total1[i]);
     element_init_Zr(total2[i],pairing);
  }
  mpz_init(bb1);
  element_init_Zr(bb2,pairing);  
  element_init_G1(bb3,pairing);  
  element_init_G1(qq1,pairing);
  element_init_G1(ans2,pairing);
  mpz_init(qq);
  mpz_init(tk);
  mpz_init(temp2);
  mpz_set_ui(temp2,0);
  gmp_randinit_default(state);
  element_init_G1(g,pairing);
  element_init_G1(h,pairing);
  element_init_G1(temp,pairing);
  element_init_G1(temp1,pairing);
  element_init_G1(temp3,pairing);
  element_init_G1(temp4,pairing);

  for(i=0;i<19;i++){
     element_random(s[i]);
     mpz_urandomb(r[i],state,32); 
  }
  for(i=0;i<=NUM;i++){
     mpz_urandomb(x[i],state,30);
     element_set0(total2[i]);
     mpz_set_ui(total1[i],0);
  }
  mpz_urandomb(tk,state,16);
  element_random(g);
  element_random(h);



  for(i=0;i<19;i++){
     element_pow_zn(temp,g,s[i]);
     element_pow_mpz(temp1,h,r[i]);
     element_mul_zn(v[i],temp,temp1);

  }



  for(j=0;j<NUM;j++){
  k=9;
  mpz_urandomb(tk,state,16);
  for (i = 1; i <= d; i++) {
                ans=cx(d-i,k);
		if (mpz_cmp_ui(tk,ans)>=0) {
			y[i - 1] = 1;
                        mpz_sub_ui(tk,tk,ans);
			k = k - 1;
		}
		else y[i - 1] = 0;
   }
  for(i=0;i<19;i++){
     if(y[i]==0){
       element_add(total2[j],total2[j],s[i]);
       mpz_add(total1[j],total1[j],r[i]);
     }
  }
  }


  t0 = pbc_get_time();
element_set1(qq1);
for(j=0;j<NUM;j++){
  k=9;
  mpz_urandomb(tk,state,16);
  element_set1(temp3);
  element_mul_zn(temp4,d1[j],total2[j]);
  element_add(temp1,temp1,temp4);
  mpz_mul(qq,d1[j],total1[j]);
  element_add(temp2,temp2,qq);
  for (i = 1; i <= d; i++) {
                ans=cx(d-i,k);
		if (mpz_cmp_ui(tk,ans)>=0) {
      element_mul(temp3,temp3,v[i])
      mpz_sub_ui(tk,tk,ans);
			k = k - 1;
		}
		else y[i - 1] = 0;
  }
  element_pow_zn(temp3,temp3,d1[j]);
  element_mul(qq1,qq1,temp3);
}
  element_set1(ans2);
  element_pow_zn(bb2,g,temp1);
  element_pow_mpz(bb3,h,temp2);
  element_mul_zn(ans2,bb2,bb3);
  t1 = pbc_get_time();
  if(element_cmp(ans2,qq1)==0) printf("Valid!\n");




  time+=t1-t0;

  printf("average en time = %f\n", time/1000);




  for(i=0;i<19;i++){
     element_clear(s[i]);
     mpz_clear(r[i]);  
     element_clear(v[i]);
     element_clear(sig[i]);
  }
  mpz_clear(tk); 
  pairing_clear(pairing);
  return 0;
}
