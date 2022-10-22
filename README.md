# poker_simulation_cpp
# Introduction

## Activity Goal

The main goal of this activity is to design a digital position
controller for the DC servomotor provided in the laboratory. To do so,
the implementation of the controller has been designed following to
different approach: the *design by emulation* method and the *direct
digital design*.  
The former consist in a controller initially designed in
*continuous-time* domain and then discretized to obtain its
*discrete-time* counterpart, while the latter aim to immediately design
the controller in the *discrete-time* domain exploiting a discrete time
model of the DC servomotor plant to be controlled. The validation of
some controllers has been performed applying different sampling periods
\(T_S\) to evaluate also their performances decreasing the
discretization precision. Finally, an extra attention has been given to
external disturbances in particular with the static friction.

## System and Model

Both designs requires a model of the DC servomotor plant that varies
according to the design technique of the controller. For the first
approach, that is the design of position PID controller obtained by
emulation, the plant model has been obtained according to the *Lab 0
Handout*, where the derivation lead to the transfer function
\[P(s)=\frac{k_{drv}k_t}{R_{eq}(J_{eq}s+B_{eq})+k_tk_e}\cdot\frac{1}{N_s}=\frac{k_m}{T_ms+1}\cdot\frac{1}{N_s}\]
with
\[k_m=\frac{k_{drv}k_t}{R_{eq}B_{eq}+k_tk_e},\hspace{5mm}T_m=\frac{R_{eq}J_{eq}}{R_{eq}B_{eq}+k_tk_e}.\]
This model has been then implemented through *Simulink* (Matlab R2020b)
following the instruction in the *Lab 0 Assignment* and is now shown in
*Figure 1*\[1\]. The additional components of the DC motor as also been
implemented and combined with the plant. Their schematic are presented
in the Appendix *A.2*.  
Once the PID has been designed it has been then attached to the
implementation of the plant model to the well known feedback controller
structure. The other controller design technique exploited resorted to
position state-space controller both with design by emulation and direct
digital design. In this case for the derivation of the controllers it
was required to represent the plant with a state-space model
\(\Sigma=(\boldsymbol{A},\boldsymbol{B},\boldsymbol{C},D)\) with
\[\begin{array}{c}
\boldsymbol{A}=\begin{bmatrix}0 & 1 \\ 0 & -\frac{1}{T_m}\end{bmatrix}=\begin{bmatrix}0 & 1 \\ 0 & -\frac{R_{eq}B_{eq}+k_tk_e}{R_{eq}J_{eq}}\end{bmatrix},\hspace{5mm}\boldsymbol{B}=\begin{bmatrix}0 \\ \frac{k_m}{NT_m}\end{bmatrix},\vspace{3mm}\\ \boldsymbol{C} = \begin{bmatrix}1 & 0\end{bmatrix},\hspace{5mm}D=0,
\end{array}\] and state vector \(\textbf{x}=[\theta_l,\omega_l]^T\).  
Since the motor speed is not a measurable state variable we have also
considered a reduced-order state observer for its estimation. The design
in the continuous–time domain, has been done by placing the single
observer eigenvalue \(\lambda_0\) at a frequency which is 5 times larger
than the frequency of the controller eigenvalues. The equations of the
observer has been derived to \[\hat{\Sigma}=\Biggl\{
\begin{array}{l}
\dot{z}=A_0+\boldsymbol{B}_0[u,y]^T \\ \hat{\boldsymbol{x}}=\boldsymbol{C}_0z+\boldsymbol{D}_0[u,y]^T
\end{array}\] where \[\begin{array}{c}
A_0=\begin{pmatrix}-\frac{1}{T_m}-L\end{pmatrix},\hspace{5mm}\boldsymbol{B}_0=\begin{bmatrix}\frac{k_m}{NT_m} & \begin{pmatrix}-\frac{1}{T_m}-L\end{pmatrix}L\end{bmatrix},\vspace{3mm} \\ \boldsymbol{C}_0=\begin{bmatrix}0 \\ 1\end{bmatrix},\hspace{5mm}\boldsymbol{D}_0=\begin{bmatrix}0&1\\0&L\end{bmatrix}
\end{array}\] with \(L\in\mathbb{R}\) is the observer gain. For the
digital design, the continuous space-state model just presented has been
discretized using the *exact method*, to obtain the "ZOH discrete
equivalent"
\(\Sigma_d=(\boldsymbol{\Phi},\boldsymbol{\Gamma},\boldsymbol{H},J)\),
with \(J=0\) since \(D=0\). This process has been performed using the
`c2d` routine of the Control System Toolbox.

  

# Tasks, Methodologies and Results

All the simulation has been performed by resorting to discrete-time
block to implement the discrete-time controller with the sampling time
defined as a variable in the Matlab code workspace. Moreover since
Simulink doesn’t provide any specific block for the ideal sampler, it
has been implemented using a *Discrete*\(\rightarrow\)*Zero-Holder Hold*
block.

## Assignment \#1: Design by emulation

### Position PID controller

As First step it was required to numerically simulate some discretized
versions of the PID already developed in the previous laboratory
activity. The numerical simulation has been performed via *Simulink* and
the PID values, from the transfer function
\[C(z)=K_P+\frac{K_I}{s}+\frac{K_Ds}{T_Ls+1},\] has been found thanks to
the well know formulas.  
So, given the response specific in term of
\(t_{s,5\%}<0.15s\ \&\ m_p<10\%\), is easy to compute
\[\omega_n=\frac{3}{\delta t_{s,5\%}},\hspace{5mm} \delta=\frac{\log{(\frac{1}{m_p})}} {\sqrt{\pi^2+\log^2{(\frac{1}{m_p})}}},\]
and by the Bode’s Method with crossover frequency \(\omega_{gc}\) and
phase margin \(\varphi_m\)
\[\omega_{gc}=\omega_n=\frac{3}{\delta t_{s,5\%}},\hspace{5mm} \varphi_m=\arctan{\frac{2\delta}{\sqrt{\sqrt{1+4\delta^4}-2\delta^2}}}\]
it was possible to obtain the solution \[\Bmatrix\begin{array}{l}
K_P = \Delta K \cos{\Delta\varphi} \\ 
T_D = \frac{\tan{\Delta \varphi+\sqrt{(\tan{(\Delta\varphi)})^2+4/\alpha}}}{2\omega_{gc}} \\
T_I = \alpha T_D
\end{array}\] where
\(\Delta K = |P(j\omega_{gc})|^{-1}, \Delta\varphi=-\pi+\varphi_m-\phase{P(j\omega_{gc})}\)
and \(\alpha\) is an extra constraint chosen arbitrary to \(\alpha>4\)
to solve the system. Moreover, \(K_D = K_PT_D\) and \(K_I=K_P/T_I\).

#### Backward Euler Method

The first approach we used was the **Backward Euler** discretization
method according to the map \(s=\frac{z-1}{T_sz}\), so that the transfer
function of our traditional PID in continuous-time *(6)* has been
discretized as
\[C(z)=K_P+K_I\frac{Tz}{z-1}+K_D\frac{z-1}{(T_L+T)z-T_L}\] with the time
constant \(T_L\) of the "real derivative" in the implementation chosen
as \(T_L=1/5\omega_{gc}\), where \(\omega_{gc}\) is the gain crossover
frequency of the continuous time control design.  
As usual the requirement were to have the *settling time* to be lower
than \(t_{s,5\%}\leq0.15s\) and an *overshoot* of at most
\(m_p\leq10\%\). Already in the numerical simulation, however, it was
immediate how the discretized PID design was not able to follow the
requirement.  
The traditional architecture, in continuous time, of this type of
controller, indeed, has already proven that it is not able to respect
performances so high for our plant. The simulation gave us the idea of
how the real system would behave.

  

The schematic adopted for the controller and the response of the real
*DC servomotor* is shown in *Figure 2*. As the simulation anticipated
the traditional PID is not able to respect the required specific, in
particular when asking the response to settle in to short time the
overshoot arise too much and, in practical application, this may
generate dangerous scenario. For the reported case the sampling time was
\(T_s=0.001\) and the "best" result we were able to achieve, giving the
priority to speed was with a settling time of \(t_{s,5\%}=0.223s\) and
an overshoot \(m_p=27.8\%\).  
The simulation and the validation has been also performed with other
sampling time, specifically with \(T_s=0.01s\) and \(T_s=0.05s\), the
time response is reported in Appendix in *Figure ???*. In the first case
the overshoot was almost doubled, \(m_p=43.6\%\) with a similar settling
time \(t_{s,5\%}=0.230s\), while in the second case the response was
unstable. This performance degradation is clearly caused by the
increased information lost due to larger sampling times.

#### *Forward Euler Method*

While the backward Euler method aim to approximate the state evolution
with the future value, which lead to the map described above. The
Forward method makes an approximation with the current states values,
meaning with the evolution given by \(f(\boldsymbol{x}(kT),e(kT))\).

  

Following this method we get the map for the approximation of our
controller given by \(s=\frac{z-1}{T_s}\) leading to the implementation
of the controller shown in *Figure 3.a*. Almost all the consideration
done for the previous method can be applied here as well, with an
additional attention given by the fact that this method map the values
in the *z-plane* also outside the unit circle meaning that the
discretization may lead to an unstable version of the controller even if
the original continuous time was stable. By the result on both the
simulation and, in particular, on the real system, however, has been
observed a behaviour similar to the backward method. The response with
sampling time \(t_s=0.001s\), shown in *Figure 3.b*, bring to slightly
better performance than the previous method with *overshoot* of
\(m_p=26.0\%\) and settling time \(t_{s,5\%}=0.229s\) but still too far
for the required performance. The other responses, for sampling time
\(T_s=0.01s\) and \(T_s=0.05s\) reported in *Figure ???*, as in the
previous case due to the loss of information for larger sampling time,
report a deterioration in performance. The former had an overshoot
\(m_p=33.6\%\) and settling time \(t_{s,5\%}=0.220s\), while for the
latter, as in the backward case, the controller became unstable.

#### *Tustin’s Method*

As we were able to see from theory, in Backward Euler and Forward Euler
method only the next or the current state is considered for the
approximation of the PID discretization. Tustin’s discretization method
on the other hand resort in a sort of interpolation between the two
values where the discretization map, given by
\(s=\frac{1}{T}\frac{z-1}{\alpha z+1-\alpha}\), consider \(\alpha=1/2\)
and so to get \(s=\frac{2}{T}\frac{z-1}{z+1}\).

  

It is interesting to notice how the Tusitn’s method is able to perfectly
map the left-half \(s\)-plane onto the unitary circle on the \(z\)-plane
leading to the expectation of better stability result, in particular
when considering sample times where the other methods fail. The map to
the \(z\)-plane just described lead to the implementation of the PID
reported in *Figure 4.a* that, as usual, has been tested at different
sampling time. For \(T_s=0.001s\), show in *Figure 4.b*, clearly we got
the best result for this approximation with an overshoot \(m_p=28.2\%\)
and a settling time \(t_{s,5\%}=0.219s\) while if \(T_s=0.01s\), *Figure
???*, the performance get worse as in the other method with overshoot
\(m_p=39.0\%\) and settling time \(t_{s,5\%}=0.220s\). An interesting
result was observed for \(T_s=0.05s\) where contrary to other methods in
this case the PID preserved its stability, providing an overshoot
\(m_p=66.0\%\) and settling time \(t_{s,5\%}=0.450s\) this improvement
of the stability might be due to the good map of the left-half
\(s\)-plane onto the unitary circle in the \(z\)-plane.

#### *Exact Emulation Method*

The Exact Emulation method is the last approach used to discretize our
traditional PID in continuous time. While the method previously adopted
consisted in a map of the \(s\)-plane onto the \(z\)-plane in this case
the transfer function of the discrete time controller is found directly
by applying the \(\mathcal{Z}\)-transform to the transfer function of
the controller in continuous time, i.e.
\(C(z)=(1-z^{-1})\mathcal{Z}\Bigl\{\mathcal{S}_T\Bigl\{\mathcal{L}^{-1}\Bigl\{\frac{C(s)}{s}\Bigl\}\Bigl\}\Bigl\}\).
The respective transfer function has been found thanks to the `c2d`
routine of the Control System Toolbox in *Matlab*.

  

The numerator and denominator returned by this routine command has been
simply implemented by a transfer function block on *Simulink*. The
validation was again been done at different sampling time, and the
response in this case present an interesting difference with respect
with the other method developed. In particular while testing with
sampling time \(T_s=0.001s\) and \(T_s=0.01s\), respectively reported in
*Figure 5.a* and *Figure 5.b*, it comes to the eye that the difference
between the two overshoot is minimal. Precisely the first validation has
reported and overshoot \(m_p=16.0\%\) with settling time
\(t_{s,5\%}=0.234s\) and the larger sampling time also returned similar
result, specifically \(m_p=26.7\%\) and \(t_{s,5\%}=0.250s\). This
similarity in performances can be explained due the fact of Exact
translation for the controller, however if the sample time again become
too large we still incur in a loss of information which bring the system
to be unstable, as for the case of \(T_s=0.05s\) shown in *Figure ???*.

#### *Anti-Windup PID*

Up to now, the traditional PID has shown how important limitation arise
when required specific become too strict. In particular we expect that
some problem may arise when the actuation control command reach its
saturation limit generating some non linear overshoot in the integral
action.

  

This issue may be particular evident when dealing with abrupt changes
such a step, as shown in the previous validation, leading to larger
overshoot and longer settling time. To overcome this problem the
*anti-windup* architecture, which is able to deal with effects of the
saturation, has been adopted implemented as in *Figure 6.a*.
Specifically, it modify the integrator input when saturation of the
output arise becoming \(e(t)=-K_A(u_C(t)-u(t))\) with
\(K_A=\frac{3}{t_{s,5\%}K_I}\) for initial validation and then adjusted
by trial and error. The new PID architecture has been tested looking for
the same performances as the previous but it was required to reach a
larger angle \(\theta_{ref}=360^{\circ}\) an with sampling time
\(T_S=0.01s\). The nominal values of \(K_P, K_I, K_D, K_A\) as then been
adjusted to reach the best performance possible, that has been done with
\(K_I'=1.01*K_I\), \(K_D'=0.01K_D\) and \(K_A'=19K_A\) leading to the
result reported in *Figure 6.b*. It was interesting to note how the
performances are sensibly increased with an overshoot \(m_p=4.9\%\) and
settling time \(t_{s,5\%}=0.170\) reached also thanks the low
overshoot.  

#### *Feedforward PID*

  
  

As last a different control technique that PID can resort is with the
feedforward control. In our case, from a reference control, we aimed to
compensate known disturbances such inertia component, BEMF component and
friction component. Starting from a reference acceleration \(a_l^*\) and
deriving the respective angular velocity \(\omega_l^*\) and the position
reference \(\theta_l^*\) which is also used as position reference of the
PID controller. The feedforward implementation for our case generate an
output \(u_{ff}\), that will be successively added to the PIC controller
output \(u_c\), that is
\[u_{ff}=\frac{NR_{eq}J_{eq}}{k_{drv}k_t}\frac{d\omega_l^*}{dt}+\frac{R_{eq}}{k_{drv}k_tN}\bigr[N^2B_{eq}\omega_l^*+\tau_{sf}sign(\omega_l^*)\bigr]+\frac{Nk_e}{k_drv}\omega_l^*.\]
The schematic is reported in *Figure 7.a* and it was validate with an
input acceleration of \[a_l^*(kT)
\Bmatrix\begin{tabular}{ll}
900 rpm/s & if 0s\leqkT<0.5s\\
0 rpm/s & if 0.5s\leqkT<1s\\
-900 rpm/s & if 1s\leqkT<2s\\
0 rpm/s & if 2s\leqkT<2.5s\\
900 rpm/s & if 2.5s\leqkT<3s
\end{tabular}\] leading to a derivation of a similar sinusoidal
reference position. The validation has been done looking for the
improvement brought by the feedforward compensation and indeed, looking
the error, *Figure 7.b, 7.c* of the two different controller is evident
how better perform the new architecture in both reactivity and precision
in tracking the reference input.

#### An extra consideration on static friction

In all the experiment we highlighted performances in term of overshoot
and settling time. It is also important, however, to notice that in
almost all the controller at steady state a kind of static error is
always present. This is due the static friction of the motor which
brings a constant error on the output. The integral action, however that
is not always able to perfectly deal with this kind of error in discrete
time. This constant error in fact is accumulated by the integrator until
a certain threshold that give the extra command to control the motor
that however is too much and so it generates this slight continuous
error around it’s reference position (as in result displayed in almost
all the experiment). This difference however is often negligible and in
some cases, as for instance with Exact and Tustin’s method at
\(T_S=0.001s\), it wasn’t neither evident it’s presence.

### Position state–space controller

Up to now all the discrete time controller were expressed by their
transfer function \(C(z)\). The classical PID shown some limitation on
performances and the also the trial and error tuning is often not
enough. A different way to design the controller is by considering its
state space model and by so resorting to pole placement in the transfer
function of the *closed-loop*. In this case, however, it was necessary
to consider the plant not fully accessible and so a reduced order
observer, to estimate the velocity \(\omega_l\) which is not directly
measurable, has been implemented in the control loop.

### Nominal Perfect tracking

  

The design of the position state-space controller has been obtained from
a previous activity, aiming to tracking a step reference input
\(r(t)=r_\infty\delta_{-1}\) in the ideal situation of perfect knowledge
of the plant dynamics and without any disturbances. Initially we found
the matrix \(K\) starting from the closed loop specifics, that this
nominal perfect steady state tracking of step position (load side)
references and step response (load side) with settling time
\(ts,5\% \leq 0.15s\) and overshoot \(M_p \leq 10\%\). For the choice of
the eigenvalues has been used an approximation of the dynamics of the
closed-loop that, given by
\[T(s)=\frac{w_n^2}{s^2+2\delta\omega_ns+\omega_n^2},\hspace{5mm}0\leq\delta<1,\]
with eigenvalues
\(\lambda_{1,2}=-\delta\omega_n\pm j\omega_n\sqrt{1-\delta^2}\) where
the determination of the natural frequency \(\omega_n\) and the damping
factor \(\delta\) that satisfy the design specifications:
\[t_{s,5\%}=\frac{3}{\delta\omega_n},\hspace{5mm}\delta=\frac{\log(1\M_p)}{\sqrt{\pi^2+\log^2(1/M_p)}}.\]
To find, on the other hand, the input feedforward gain
\(N_u\in\mathbb{R}\) and the state feedforward gain
\(\boldsymbol{N}_x\in\mathbb{R}^{n\times1}\) it is sufficient to solve
the system of linear equations \[\begin{bmatrix}
        \boldsymbol{A} & \boldsymbol{B} \\ \boldsymbol{C} & D
    \end{bmatrix}
    \begin{bmatrix}
        \boldsymbol{N}_x \\ N_u
    \end{bmatrix}
    =
    \begin{bmatrix}
        \boldsymbol{0} \\ 1
    \end{bmatrix}
    .\] By that, we defined the control law
\[u=-\boldsymbol{Kx}+(N_u+\boldsymbol{KN}_x)r\] which lead to the
implementation via Simulink as represented in *Figure 8.a*. Once found
the control law and the eigenvalues that should bring the closed loop to
respect the required specific, it was also necessary to design an
estimator for the state
\(\boldsymbol{x}=[\theta_l\hspace{2mm} \omega_l]^T\) since the angular
velocity is not directly measurable. The representation of the DC
gearmotor is already partitioned in such way that the reduced order
state observe can be immediately derived by setting
\(\boldsymbol{T}=\boldsymbol{I}_{n\timesn}\), moreover,
\(\boldsymbol{A}'=\boldsymbol{A}\), \(\boldsymbol{B}'=\boldsymbol{B}\),
\(\boldsymbol{C}'=\boldsymbol{C}\) and \(D'=D\). This lead to the
reduced order state observe to be a dynamical system whose represented
by \[\hat{\Sigma}=\Biggl\{
\begin{array}{l}
\dot{z}=A_0+\boldsymbol{B}_0[u,y]^T \\ \hat{\boldsymbol{x}}=\boldsymbol{C}_0z+\boldsymbol{D}_0[u,y]^T
\end{array}\] where \[\begin{array}{c}
A_0=\begin{pmatrix}-\frac{1}{T_m}-L\end{pmatrix},\hspace{5mm}\boldsymbol{B}_0=\begin{bmatrix}\frac{k_m}{NT_m} & \begin{pmatrix}-\frac{1}{T_m}-L\end{pmatrix}L\end{bmatrix},\vspace{3mm} \\ \boldsymbol{C}_0=\begin{bmatrix}0 \\ 1\end{bmatrix},\hspace{5mm}\boldsymbol{D}_0=\begin{bmatrix}0&1\\0&L\end{bmatrix}
\end{array}\] with \(L\in\mathbb{R}\), or the observer gain, found such
that the eigenvalues of \(A_o=A_{22}'+LA_{12}'\) is located at a desired
position, so in our case 5 times larger than \(\omega_n\). The
implementation of this observer is shown in *Figure 8.b*.

### Robust tracking - Integral Action

![State Space Robust Controller Simulink
Implementation](StateSpace_ControllerRobust.JPG)

To guarantee perfect tracking of out step reference signal, considering
also uncertainties and constant disturbance, can be done by adding an
integral action in the control law. This action can be included to the
control law, in continuous time as
\[u=u_\infty-\boldsymbol{K}(\boldsymbol{x}-\boldsymbol{x}_\infty)-K_I\int_{0}^{t}[y(\tau)-r(\tau)] \,d\tau\]
leading to the control scheme in *Figure 9*.  
The introduction of the integrator state bring us to the extended system
\[\Sigma_e:\Bmatrix\begin{tabular}{c}
     \begin{bmatrix}
     \dot{x}_I(t)\\ \dot{\boldsymbol{x}}(t)
     \end{bmatrix}
     = \underbrace{\begin{bmatrix}
         0 & \boldsymbol{C} \\ \boldsymbol{0} & \boldsymbol{A}
     \end{bmatrix}}_{\triangleq \boldsymbol{A}_e}
     \underbrace{
     \begin{bmatrix}
     x_I(t)\\ \boldsymbol{x}(t)
     \end{bmatrix}}_{\triangleq \boldsymbol{x}_e}
     +
     \underbrace{
     \begin{bmatrix}
     0 \\ \boldsymbol{B}
     \end{bmatrix}}_{\triangleq \boldsymbol{B}_e}
     -
     \begin{bmatrix}
     1 \\ \boldsymbol{0}
     \end{bmatrix}r(t)
     \\
     y(t)=\underbrace{\begin{bmatrix}
         0 & \boldsymbol{C}
     \end{bmatrix}}_{\triangleq \boldsymbol{C}_e}
     \begin{bmatrix}
         x_I(t)\\ \boldsymbol{x}(t)
     \end{bmatrix}
\end{tabular}\] and the state-feedback matrix for the augmented state is
\(\boldsymbol{K}_e=\begin{bmatrix}K_I & \boldsymbol{K}\end{bmatrix}\),
where it’s proper choice allows the arbitrary placement of the
eigenvalues of the closed loop system
\[\boldsymbol{A}_{cl}\triangleq\boldsymbol{A}_e
-\boldsymbol{B}_e\boldsymbol{K}_e
=\begin{bmatrix}
    0 & \boldsymbol{C} \\
    -\boldsymbol{B}K_I & \boldsymbol{A}-\boldsymbol{B}\boldsymbol{K}
\end{bmatrix}.\] The poles placement has already been done in a previous
activity where they were chosen to be
\(\lambda_{1,2,3}=2.1\delta\omega_n\). Since also in this case the state
of the system is considered not fully observable a reduced order
observer, as for the nominal case shown in *Figure 8.b*, has been added
in the control scheme on Simulink.

### Discretization and Validation

Once done the design for the controller and the observer, before moving
to the validation, it was necessary to discretize what has been just
designed. To do so the Forward Euler method has been adopted which
consist in the transformation of a continuous state space model
\(\Sigma_c=(\boldsymbol{A},\boldsymbol{B},\boldsymbol{C},\boldsymbol{D})\)
to its discretized version
\(\Sigma_d=(\boldsymbol{\Phi},\boldsymbol{\Gamma},\boldsymbol{H},\boldsymbol{J})\)
with \(\boldsymbol{\Phi}=\boldsymbol{I}+\boldsymbol{A}T_s\),
\(\boldsymbol{\Gamma}=\boldsymbol{B}T_s\),
\(\boldsymbol{H}=\boldsymbol{C}\) and
\(\boldsymbol{J}=\boldsymbol{D}\).  
Regarding the control law it has been expressed in term of its gain
given by the matrices \(\boldsymbol{N}_x\), \(\boldsymbol{N}_u\) or
\(\boldsymbol{N}_r=\boldsymbol{N}_u+\boldsymbol{K}\boldsymbol{N}_x\) and
the only the only precaution to do is to insert a discrete time
integrator instead of a continuous one. On the other hand the observer
must be discretized, however, we easily obtain its discretized version
to be given by
\(\boldsymbol{\Phi}_o=\boldsymbol{I}+\boldsymbol{A}_oT_s\),
\(\boldsymbol{\Gamma}_o=\boldsymbol{B}_oT_s\),
\(\boldsymbol{H}_o=\boldsymbol{C}_o\) and \(J_o=D_o=0\).  
The validation has been performed with, as usual, three different
sampling time, specifically with \(T_1=0.001s\), \(T_2=0.01s\) and
\(T_3=0.05s\) and the result obtained are remarkable. It’s immediate,
especially if compared with the PID response, how better perform both in
term of settling time and in the overshoot. For the nominal case the
response has an overshoot \(m_p=3.7\%\) and settling time
\(t_{s,5\%}=0.086\%\) and for the robust architecture the result are
even better with \(m_p=0.1\%\) and \(t_{s,5\%}=0.065\%\). In the nominal
architecture however is evident a large displacement from the reference
position. This difference may be due the assumption of the perfect
knowledge of the system and therefore omitting the presence of load
disturbances. The robust design, on the other hand, has the integral
action which takes charge of this phenomena and by that its response is
much closer to the desired one, with just a small difference probably
due to static friction disturbance that we already observed with the PID
controller. When the sampling time is increased also with these methods
we can easily predict a worsening of the performance, as already see in
other cases. From the result obtained, however is possible to see that
the result are yet acceptable in both cases for the sampling time
\(T_s=0.01s\), reported in *Figure ???*, where for the nominal case the
response has an overshoot \(m_p=9.4\%\) and settling time
\(t_{s,5\%}=0.170s\) which is slightly above the requirement, while the
robust design is still far better than the maximum performance allowed,
with \(m_p=1.2\%\) and \(t_{s,5\%}=0.060s\) When, nevertheless, the
sampling time become too large the loss of information, also in this
case become too much leading to an instability of the control systems.
This behaviour has been seen specifically in both nominal and robust
tracking for \(T_s=0.05s\), *Figure ???*.

  

## Assignment \#2: Direct Digital Design

As already introduced the direct digital design aim to design a
controller starting from the discretized version of the DC servomotor
plant model. To do so, the already derived state-space model
\(\Sigma_c=(\boldsymbol{A},\boldsymbol{B},\boldsymbol{C},D)\) has been
discretized using the exact method, returning the "ZOH discrete
equivalent"
\(\Sigma_d=(\boldsymbol{\Phi},\boldsymbol{\Gamma},\boldsymbol{H},J)\)
from the `c2d` routine of the Control System Toolbox. Again has been
adopted a nominal design, on the assumption of perfect knowledge of the
system and no external noise, and the robust, which take into account
the presence of external interference such load disturbances.

### *Nominal Tracking*

Using the discretized plant model \(\sigma_d\) we started the design of
the discrete time position state space controller for nominal perfect
tracking. The control law, also in the case, come from the perfect
steady state tracking condition \[\Bmatrix\begin{tabular}{c}
     \boldsymbol{\Phi}\boldsymbol{x}_\infty+\boldsymbol{\Gamma}u_\infty=\boldsymbol{x}_\infty \\
     \boldsymbol{H}\boldsymbol{x}_\infty=\boldsymbol{y}_\infty=r_\infty
\end{tabular}\] which, considering
\(\boldsymbol{x}_\infty=\boldsymbol{N}_x r_\infty\) and
\(u_\infty=N_u r_\infty\), lead to the derivation of the two gains
\(N_u\in\mathbb{R}\) and \(\boldsymbol{N}_x\in\mathbb{R}^{n\times1}\)
from the system of linear equations \[\begin{bmatrix}
        \boldsymbol{\Phi-I} & \boldsymbol{\Gamma} \\ \boldsymbol{H} & 0
    \end{bmatrix}
    \begin{bmatrix}
        \boldsymbol{N}_x \\ N_u
    \end{bmatrix}
    =
    \begin{bmatrix}
        \boldsymbol{0} \\ 1
    \end{bmatrix}
    .\] Again in this situation the state, and in particular the angular
velocity is not directly measurable, therefore the design of an observer
is required to estimate it. The observer matrices are given by:
\[\begin{array}{c}
\Phi_o=\Phi_{22}-L\Phi_{12},\hspace{5mm}\boldsymbol{\Gamma}_o=\begin{bmatrix}\Gamma_2-\Gamma_1&(\Phi_{22}-L\Phi_{12})L+\Phi_{12}-L\Phi_{11}\end{bmatrix},\vspace{5mm}\\
\boldsymbol{H}_o=\begin{bmatrix}0\\1\end{bmatrix},\hspace{5mm}\boldsymbol{J}_o=\begin{bmatrix}0&1\\0&L\end{bmatrix}   
\end{array}\] where \[\boldsymbol{\Phi}=\begin{bmatrix}
    \Phi_{11} & \Phi_{12} \\ \Phi_{21} & \Phi_{22}
\end{bmatrix},\hspace{5mm}\boldsymbol{\Gamma}=\begin{bmatrix}
    \Gamma_1 \\ \Gamma_2
\end{bmatrix}.\] The matrix \(L\) is the observer gain and is chosen
such that the eigenvalues of the of \(\Phi_o\) is placed at some desired
position. For our case, however, since the not observable state is only
one, both \(\Phi_o\) and \(L\) are both scalar. For the discrete time
regulator just found is required to have the same response as the
continuous time regulator and so the discrete eigenvalues are chosen
according to the transformation \(z=e^{sT}\).

### *Robust Tracking - Integral Action*

The design of the controller for robust tracking can be done following
the procedure of continuous time controller but cosnidering the
discretized version of the model \(\Sigma_d\). The discrete time control
law with integral action is given by \[\Bmatrix\begin{array}{c}
     x_I[k+1]=x_I[k]+(y[k]-r[k])\\
     u[k]=-K_Ix_I-\boldsymbol{K\hat{x}}[k]+\boldsymbol{N}_r[k]
\end{array}\] where \(\boldsymbol{N}_r[k]=N_U+\boldsymbol{KN}_x\) is the
reference feedforward gain, computed in the same way described in
*(11)*. The integrator state, then, has been combined to create the
extended augmented state system \[\begin{tabular}{c}
     \Sigma_e:\begin{bmatrix}
     x_I[k+1]\\ \boldsymbol{x}[k+1]
     \end{bmatrix}
     = \underbrace{\begin{bmatrix}
         0 & \boldsymbol{H} \\ \boldsymbol{0} & \boldsymbol{\Phi}
     \end{bmatrix}}_{\triangleq \boldsymbol{\Phi}_e}
     \underbrace{
     \begin{bmatrix}
     x_I[k]\\ \boldsymbol{x}[k]
     \end{bmatrix}}_{\triangleq \boldsymbol{x}_e}
     +
     \underbrace{
     \begin{bmatrix}
     0 \\ \boldsymbol{\Gamma}
     \end{bmatrix}}_{\triangleq \boldsymbol{\Gamma}_e}
     -
     \begin{bmatrix}
     1 \\ \boldsymbol{0}
     \end{bmatrix}r(t).
\end{tabular}\] The state feedback matrix can be now imposed to be
\(K_e=\begin{bmatrix}K_I, & \boldsymbol{K}\end{bmatrix}\) chosen so that
eigenvalues of the closed-loop system
\[\boldsymbol{\Phi}_{cl}\triangleq\boldsymbol{\Phi}_e-\boldsymbol{\Gamma}_e\boldsymbol{K}_e=\begin{bmatrix}
    1 & \boldsymbol{H} \\ -\boldsymbol{\Gamma}\boldsymbol{K}_I & \boldsymbol{\Phi}-\boldsymbol{\Gamma}\boldsymbol{K}
\end{bmatrix}\] are placed to the desired position. The design of the
observer has been done in the same way than for the nominal tracking and
the same reasoning of the regulator eigenvalues choice is applied,
meaning that they have been obtained mapping the eigenvalues of the
continuous time design on the \(z\)-plane according to the
transformation \(z=e^{sT}\).

  

  

### Discretization and Validation

The design implementation is the same as for the state space emulation
method, therefore the implementation of the controller for nominal
tracking is shown in *Figure 8.a* and for the robust tracking in *Figure
9*. The same structure of the observer has been used for both the design
and it is shown in *Figure 8.b*.  
The validation has been, as usual, done at three different sampling
step, again at \(T_1=0.001s\), \(T_2=0.01s\) and \(T_3=0.05s\). Starting
from the design for nominal tracking the difference that immediately
catches the eye is that for the faster sampling time the result are
closer to the continuous time state space design by emulation, with
overshoot \(m_p= 3.7\%\) and settling time \(t_{s,5\%}=0.084s\). When,
however, has been done the validation at larger sampling time we
immediately observed that the tracking performances are not getting much
worse compared to the changes in the previous method. For the case with
\(T_s=0.01s\), in fact the results were remarkably inside the specific
requirements with \(m_p=5.1\%\), \(t_{s,5\%}=0.140s\). For the last
sampling time \(T_s=0.05\), where we were used to see the system to
become unstable, in this case the closed loop system remains stable,
even if the performances exceed the requirements, with \(m_p=21.7\%\),
\(t_{s,5\%}=0.350s\). All the validation for the nominal tracking,
however, show the same problem already seen in the continuous time state
space design that is a constant displacement at steady state due the
assumption of perfect knowledge of the system without considering any
external disturbances. The design for robust tracking also include the
integral action that takes into account the issue just cited, and
therefore bringing the steady state error closer to zero net of the load
friction. This extra implementation however lead to worse performance
primarily in term of overshoot and consequently also the settling time.
Specifically we got for \(T_s=0.001s\) \(m_p=19.9\%\) and
\(t_{s,5\%}=0.390s\), for \(T_s=0.01s\) \(m_p=22.0\%\) and
\(t_{s,5\%}=0.410s\) and for \(T_s=0.05s\) \(m_p=54.8\%\) and
\(t_{s,5\%}=0.400\). It is also interesting to observe how the worsening
of the performance was similar as for the nominal case, with a similar
overshoot from \(T_s=0.001s\) to \(T_s=0.01s\) and settling time close
in all the situation. The preservation of stability also for the higher
sampling time might be due the fact the the controller has been designed
for an already discretized plant, and it has not been translated from
continuous time to discrete time as before, making it less sensitive to
this change.

# Appendix

## Matlab code

## Data sheet

## Detailed calculations for PID design

  

  

  

![Discretized Exact PID Time Response with
\(T_S=0.05s\)](PID_Exact_Ts05.jpg)

  

  

1.  The Datasheet of the plant scheme is reported in appendix *A.2*
