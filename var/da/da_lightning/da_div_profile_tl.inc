subroutine da_div_profile_tl(grid, info, n, k, div)

   !--------------------------------------------------------------------------
   ! Purpose: Calculates divergence (div) on each level at the observed location (i,j). 
   ! dx, dxm, dy, dym are horizontal interpolation weighting.
   !                        d   U      d   V
   !           Div = m^2 *[---(---) + ---(---) ] 
   !                        dx  m      dy  M
   ! Authors: Z Chen (zchen@fjnu.edu.cn), Jenny Sun (NCAR), X Qie (IAP)
   !--------------------------------------------------------------------------

   implicit none

   type (domain),         intent(in)  :: grid
   type (infa_type),      intent(in)  :: info
   integer,               intent(in)  :: n, k
   real,                  intent(inout) :: div
                          
   integer :: ii, jj    ! index dimension.
   real    :: div_m(2,2)     ! divergence

   integer :: i, j      ! OBS location
   real    :: dx, dxm   ! interpolation weights.
   real    :: dy, dym   ! interpolation weights.
   real    :: coeff  
   
   if (trace_use_dull) call da_trace_entry("da_div_profile_tl")

   i   = info%i(1,n)
   j   = info%j(1,n)
   dx  = info%dx(1,n)
   dy  = info%dy(1,n)
   dxm = info%dxm(1,n)
   dym = info%dym(1,n)

   ! calculate layered divergence
   if(i == its) i = its + 1
   if(i == ite) i = ite - 1
   if(j == jts) j = jts + 1
   if(j == jte) j = jte - 1

   do ii = i, i+1
      do jj = j, j+1		 
         coeff = grid%xb%map_factor(ii,jj)*grid%xb%map_factor(ii,jj)*0.5/grid%xb%ds
		 
         div_m(ii-i+1,jj-j+1) = (grid%xa%u(ii+1,jj,k)/grid%xb%map_factor(ii+1,jj) - &
		                 grid%xa%u(ii-1,jj,k)/grid%xb%map_factor(ii-1,jj) + &
				 grid%xa%v(ii,jj+1,k)/grid%xb%map_factor(ii,jj+1) - &
				 grid%xa%v(ii,jj-1,k)/grid%xb%map_factor(ii,jj-1))* coeff
      end do
   end do

   ! Horizontal interpolation to the obs. pt.
   div = dym*(dxm*div_m(1,1)+dx*div_m(2,1))+dy*(dxm*div_m(1,2)+dx*div_m(2,2))

   if (trace_use_dull) call da_trace_exit("da_div_profile_tl")

end subroutine da_div_profile_tl
