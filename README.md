h# Mix2dSP
A small utility with OpenGL capabilities used to calculate nonlinear optical interactions of three waves.
Physical model: wave stands for a light beam with intensity profile of a Gaussian function. The beams are FFT-ed in time and space, and the obtained plane waves are propagated through the nonlinear medium. THe interaction between the plane waves of the considered lights beams are modeled by a set of nonlinear coefficients describing the conversion ratio from one k-vector into another.

Computer model: 
propagation in space is regarded as multitude of steps that in turn are split into one linear propagation step and one nonlinear propagation step. Each step is calculated through the solution of linear part of the differential equations and the nonlinear part separately. Adustments to the results of each step (to account for the effect of separation in linear and nonlinear parts) currenlty are not implemented.

grpahical presentation of the results is implemented through the use of OpenGL. Labeling of data points is still under the development.
