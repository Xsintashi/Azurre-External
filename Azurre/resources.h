#pragma once

#include <array>
#include <cstddef>

namespace resource {

	template <std::size_t N>
	constexpr auto decodeBase85(const char(&input)[N]) noexcept
	{
		std::array<char, N * 4 / 5> out{};

		constexpr auto decode85Byte = [](char c) constexpr -> unsigned int { return c >= '\\' ? c - 36 : c - 35; };

		for (std::size_t i = 0, j = 0; i < N - 1; i += 5, j += 4) {
			unsigned int tmp = decode85Byte(input[i]) + 85 * (decode85Byte(input[i + 1]) + 85 * (decode85Byte(input[i + 2]) + 85 * (decode85Byte(input[i + 3]) + 85 * decode85Byte(input[i + 4]))));
			out[j] = ((tmp >> 0) & 0xFF); out[j + 1] = ((tmp >> 8) & 0xFF); out[j + 2] = ((tmp >> 16) & 0xFF); out[j + 3] = ((tmp >> 24) & 0xFF);
		}

		return out;
	}

	inline constexpr auto plantedC4 = decodeBase85(
		"=pxq9_8v7&0C/2'K(HH=dfES7dfES715###SG=IGW%###sEPj;leHo[&d[@=b$###A<nH=M>'DNjkYxO0j]=uL^.JL69m1K1a<on@C.;6x]v%+%ImiMO)35&BSRW8CHQW8v[UiFp)fUC"
		"33/'d#[Q/>BMLFNM)^AJpok_#A;CC`2rXgJgN?9B^s#w/FW%f3Emsk#XTDTTMx(+c?p+].*Y$B(C1'K%S=pg2V'%81;B3tO3VpWs>G::m6_Fj]iA-1px0%N*7vm=2NcW,;=E]A)qI45U"
		"VK=5=Ztq.L#G)9LDMPb>4w?^[+chWd6I[tUn8RPA>tNNJU^##G&T@4Ln/rFV2hH/K&x=3#h3@Q5;41F*TSo/;iJIdfM8xRSH7Er.]J)t7nrnL5bY5t6LJxVW&%9M6bi8S5V7.li&L3Lq"
		"9T3m9SY+sNcGNBUR;Q()+A(V2SA]u4A,OV'n6CG)a?5%G^xpD4d`O.-uHg]l2NmF:/^8ljA$iFCR`QjGj0nSllLOX$$P^v1K;JeTPL>GC?L,Z#8:em0mgAtg$X-rg@[(&#*bBr?;_>eg"
		"Ck%t3#ams3f1&Cu><RrHXO+hj-d$3+MS1:(J$O@phRhW+/AM;Ol:?wO4Iu/L=jg:d36lsg+IumW0h_.iOk6e;pS:i#_?S(]-''>SmHi=3P[j-(cX@RaET3@DY3%9Q(%3H5M5DKOFi3p+"
		"CCOg;JWAkW1m?i_%NRnsiWFrH%A^5UcG0nb4=h3On;YA;LK)0d6P-wD)spSsfV=.(PNGOcOdU[g6g,rB%uwmHO='?pnrMK(,'_Xc>@pnF+VG@-']If0a,Z+vZgmFD=fw1/mOlW5o0CKM"
		"*3+_3rR%ff-lL>#(>E?QRd0E,ZtdsA.r)X1r;vkavc2=(@a.)$/d@>YPrEg7(#`5&xvs0&UQnk<dr4K3D0v.8V285fB:6Y+$bN7M5qEI;,g<3*V`:0(1)N,3D4qo-g0%OgH/Fv$<OK-8"
		"V,-I)=Vxv%Ahv68'FQ-Y>f4hYVnt7+*0)v%7p%g7=x1QD$<Q[KSC7i%L65f[W=5^+*^r0&k*Y)lR'5G`3nf_&bpd%/O>afM$TRHqovx@6-x[En*K2L(-'Io*qiNl$::7LQ]&1JZ:/WD&"
		"xg4=9=aCAm(1hber4@i*tT#q9GvF1=$dwO)J%H2^o6-hYBDg^Hrp6f4/k@i?7nQ++xMP*:.-S*g@hN;HbTHq@cwHp^^q_nFo[$N[0*H3't+iSbQ*<g2S=l)T7]sM;RtViC9)v@TC]+#r"
		"n]ZP,T928c<Q/Ui#-#uoE$HtHokCj9?L%led_L*0vBa/Qo?a(brX5?l>NLTIdorDkfM4;DgS:?Xl,hj425,h#/NND/>6cm%ZgdDbR%>gVV^'>qpKY>:[S'XZK:wm1Q8J/R:^Z>Tj429;"
		"R&-pQi0ij.Ir*pn#tCrnO+k5TN8P#6$0'p#uxGP3*Oxe(IXL29<8Z@;q$Y,8G(^b8k0/c_Ks8f=hmq=^v@0nMrKDfo_@&]IesaH*BiUKJ$LrIR_7H(Hru]h=Jp_;>,m%Q.DFgnXp>,x?"
		"coieE1h+<MBoidc7iGQUPi40=E.o40v_j$>af3EooO?:*=gs_'PN%UCpf7>NK^t%i>=>N,KYqO12-6hMM)4/<Sp8L@SDKxDTts=ssYh0N*C+a1OX6/m+22G^2N>aJS++HI')0mkQkf#v"
		"f]XXVjblLQfjuGlR$#W=7r$=X[_@3;GdHqXkF(K0Ve^P>;grN[TdFLZBZ@,[(u1&8iq;QXOE$qWFBXZ9ufo6WMtnR>38Z%Z$Lo0;aW5(g@M)[rmqME>)Z$:E72CBoQo20@?3$1@RoQ^H"
		"a:X^6Y81m`,XUY8I5]@.?bi&m/VrARlsw[_F.2(HS[Is0B$al>BBr]^/[i,Iv,?7s/];mP-hrLU2a*p_8HFOF6:3,k^O5#V^e<FA-FH>KMSQ4V^k#rKmq7xABI^LDl.aX.KY5iCE4r[P"
		"mIS%Kl`Yi5+n(GE6XqkpwvP_u7jt&R3pPeGHLeChsa5.^IM(Q3.mEg?+*S$HE4H9t?XM0r=J:kS#1ZjAFZ4[BIfl]hItvCMdR-akNce4j2Thjh>+MRMSueG2vn;gZrm=&i>-o_?T&B)^"
		"6&Hl^=L8-g^N`$OFbj=po_JVP$>wGhkbforSL=UHw5f:$wieZYan@+%ArlCobtDJ/d6ls@:DiUW,WDt;wP[GpJW6F4i]V>fw`/m,t*R2$)Y+oVfMG+Qq/XC)JbACDJM[lhss]09VUhYp"
		"wdnZ='^l24BR-E@/Ypgp87mU$V-uU6u&'qmSGC@bfW19`h=_(goHH;g,G1(a(^rR7g^Y<?UpmE-JXRV[jshBQ6wSj<f9j&5&?>6qB4TSQ$u2(W=L++gmUUi[T3cl[VZOEqK0uj%lruV("
		"T>+u-s>i/=S7G5Yml>-Sb76UZ2.lsNZ'78A,bfTJoGwK&npq4V1@s@BTLT@9W%mo,Dut]r.ih<nfqx@EJi4p1Lh:>D1rgJ5eS**dCF@bcxS$Nc@Om0/Ei-9<*B@A:x$[6GJCZwQq7U5R"
		"/asRRw.1t:0207;QK.VXbd)DqVh_wDE6^$EvZ)4oL/WUEWaZp?QO3Xlodk,+kB/O$nuu8>3D;6_KbNsDTv-5t7RZTotC]M4?WHp9Y3mAfK,+ZpXohLEV0@sq2ao[m,j&)@^Hwu?vk3@D"
		"U.mtLF'@Y6EwC.97:G-o-P+2uqPirTWj'Q*N'S:1ZZZ1]b_91WS5f:]d4t0kSScRp0c^5n6Gx&L&v/r@_9ovZAd'Kb:3aXPe0&mkK3iEaYQIK]^3L[eV<jSlX5Gb',S1gm.#$Bs'mV1<"
		"+,#nK6SqZfi:Pn)C'7QJAu?q02g2nY$M*g>6w7CZ*.Vl@QM@M?<GlYKo22.gk<8rZtg?Jc]@=$G[soUsJpOwfL@FfN`DN;q67#sBb2FlF_8=OTNmV),%B7Y6$P&'_[P59tHQP]gf<>Ni"
		"WW>Ft;^pm@oJ=14P:pOWf::#$+P]%Vf^KXu_@E0mr5$S/vsV8uAkg^gxxMVHk)I2Ipbj+I5migUYtJ*hI?]quZlDSNe3/kYk$Js_RG8`;5u?4D(S.aUX>S%HdO8oI71hSupoPCF$.ah'"
		"QJ,hIQIJ@Xx37:KH%'1A.KF1nE*w4PqI&F1C),R@402JC:<YIL1fSlHOdRg4nc4v'5d]J3VmDfu$hoA;d>K%AwfY@sNl?Gqxs'pQX(%960?hUpV'f)J:t?uc`%l29L*_]>X0n;h10Ljf"
		"o]'TWb]A)d.=l87`T<W+V%8bct;I9&F:i3>G.EI4LO@RU=UV9aC`e+h#+rif)gVjKepm;sjgp9-k8,rZ]B,'I&.&YcT3)=A4q;Mr]QaxU_1H':`[062VTFkQ<Eme1fLKC`*i7a&d?4&q"
		"J#vfK=4ADrS0N@hl.er$Z;A<uON:+(,umo%S'4)HAg6-)Ag6-)[G2?U<,>>#v<$w7<&[;pl%ClbBH:kb)?vcJ0b+Ei'*gBiA.&$4nN55]1Ito=DbX>/33)ZB=ioZ%qhh68NH_:)9O392"
		"tdo__7JcgSJ$W+#s%64#?3O?ABJ9:qZ63IKp6IuG/Z0;FhIjV7TV$]>*4AXu=/i;MSxk48aCLTr<IPT)>m_MMXW`MEE9vB#lqO^Ae#ltYGSZRl+4,AFsZ3>%cw-P1(m<;$)ZkB4/=$*H"
		"K).ErbpJk$=M0MihN$%#HY2]UJaM_P>@a_I5?<0(4tEt'5nONE[a:,&K.Zr3Ph;-'3g/snP0Isu2Sx<,Xm><-wLhT?b1Z8=hsv?ZR#--LXHaO5[^'h$+DTb7rCbw.0)Dt?Aw(+#V>44#"
		">Ds`NYVKxYV$gRJmIsnmcr9+9<@?E&oR<DECY766pcdBAb*aWI*AsMD8X^/(LOBl/MgVT*;_G$#&fDOYjlwR%'=%q`5b#]IkEoIqd].Yl]/)-#iWWJMY+Nd2SMV:#nF34$DS@J6#####"
		"D2&u8LsapL");

	inline constexpr auto C4 = decodeBase85(
		"=pxq9_8v7&0C/2'K(HH=dfES7dfES715###SG=IGW%###sEPj;leHo[&d[@=b$###A<nH=M>'DNjkYxO0j]=uL^.JL69m1K1a<on@C.;6x]v%+%ImiMO)35&BSRW8CHQW8v[UiFp)fUC"
		"33/'d#[Q/>BMLFNM)^AJpok_#A;CC`2rXgJgN?9B^s#w/FW%f3Emsk#XTDTTMx(+c?p+].*Y$B(C1'K%S=pg2V'%81;B3tO3VpWs>G::m6_Fj]iA-1px0%N*7vm=2NcW,;=E]A)qI45U"
		"VK=5=Ztq.L#G)9LDMPb>4w?^[+chWd6I[tUn8RPA>tNNJU^##G&T@4Ln/rFV2hH/K&x=3#h3@Q5;41F*TSo/;iJIdfM8xRSH7Er.]J)t7nrnL5bY5t6LJxVW&%9M6bi8S5V7.li&L3Lq"
		"9T3m9SY+sNcGNBUR;Q()+A(V2SA]u4A,OV'n6CG)a?5%G^xpD4d`O.-uHg]l2NmF:/^8ljA$iFCR`QjGj0nSllLOX$$P^v1K;JeTPL>GC?L,Z#8:em0mgAtg$X-rg@[(&#*bBr?;_>eg"
		"Ck%t3#ams3f1&Cu><RrHXO+hj-d$3+MS1:(J$O@phRhW+/AM;Ol:?wO4Iu/L=jg:d36lsg+IumW0h_.iOk6e;pS:i#_?S(]-''>SmHi=3P[j-(cX@RaET3@DY3%9Q(%3H5M5DKOFi3p+"
		"CCOg;JWAkW1m?i_%NRnsiWFrH%A^5UcG0nb4=h3On;YA;LK)0d6P-wD)spSsfV=.(PNGOcOdU[g6g,rB%uwmHO='?pnrMK(,'_Xc>@pnF+VG@-']If0a,Z+vZgmFD=fw1/mOlW5o0CKM"
		"*3+_3rR%ff-lL>#(>E?QRd0E,ZtdsA.r)X1r;vkavc2=(@a.)$/d@>YPrEg7(#`5&xvs0&UQnk<dr4K3D0v.8V285fB:6Y+$bN7M5qEI;,g<3*V`:0(1)N,3D4qo-g0%OgH/Fv$<OK-8"
		"V,-I)=Vxv%Ahv68'FQ-Y>f4hYVnt7+*0)v%7p%g7=x1QD$<Q[KSC7i%L65f[W=5^+*^r0&k*Y)lR'5G`3nf_&bpd%/O>afM$TRHqovx@6-x[En*K2L(-'Io*qiNl$::7LQ]&1JZ:/WD&"
		"xg4=9=aCAm(1hber4@i*tT#q9GvF1=$dwO)J%H2^o6-hYBDg^Hrp6f4/k@i?7nQ++xMP*:.-S*g@hN;HbTHq@cwHp^^q_nFo[$N[0*H3't+iSbQ*<g2S=l)T7]sM;RtViC9)v@TC]+#r"
		"n]ZP,T928c<Q/Ui#-#uoE$HtHokCj9?L%led_L*0vBa/Qo?a(brX5?l>NLTIdorDkfM4;DgS:?Xl,hj425,h#/NND/>6cm%ZgdDbR%>gVV^'>qpKY>:[S'XZK:wm1Q8J/R:^Z>Tj429;"
		"R&-pQi0ij.Ir*pn#tCrnO+k5TN8P#6$0'p#uxGP3*Oxe(IXL29<8Z@;q$Y,8G(^b8k0/c_Ks8f=hmq=^v@0nMrKDfo_@&]IesaH*BiUKJ$LrIR_7H(Hru]h=Jp_;>,m%Q.DFgnXp>,x?"
		"coieE1h+<MBoidc7iGQUPi40=E.o40v_j$>af3EooO?:*=gs_'PN%UCpf7>NK^t%i>=>N,KYqO12-6hMM)4/<Sp8L@SDKxDTts=ssYh0N*C+a1OX6/m+22G^2N>aJS++HI')0mkQkf#v"
		"f]XXVjblLQfjuGlR$#W=7r$=X[_@3;GdHqXkF(K0Ve^P>;grN[TdFLZBZ@,[(u1&8iq;QXOE$qWFBXZ9ufo6WMtnR>38Z%Z$Lo0;aW5(g@M)[rmqME>)Z$:E72CBoQo20@?3$1@RoQ^H"
		"a:X^6Y81m`,XUY8I5]@.?bi&m/VrARlsw[_F.2(HS[Is0B$al>BBr]^/[i,Iv,?7s/];mP-hrLU2a*p_8HFOF6:3,k^O5#V^e<FA-FH>KMSQ4V^k#rKmq7xABI^LDl.aX.KY5iCE4r[P"
		"mIS%Kl`Yi5+n(GE6XqkpwvP_u7jt&R3pPeGHLeChsa5.^IM(Q3.mEg?+*S$HE4H9t?XM0r=J:kS#1ZjAFZ4[BIfl]hItvCMdR-akNce4j2Thjh>+MRMSueG2vn;gZrm=&i>-o_?T&B)^"
		"6&Hl^=L8-g^N`$OFbj=po_JVP$>wGhkbforSL=UHw5f:$wieZYan@+%ArlCobtDJ/d6ls@:DiUW,WDt;wP[GpJW6F4i]V>fw`/m,t*R2$)Y+oVfMG+Qq/XC)JbACDJM[lhss]09VUhYp"
		"wdnZ='^l24BR-E@/Ypgp87mU$V-uU6u&'qmSGC@bfW19`h=_(goHH;g,G1(a(^rR7g^Y<?UpmE-JXRV[jshBQ6wSj<f9j&5&?>6qB4TSQ$u2(W=L++gmUUi[T3cl[VZOEqK0uj%lruV("
		"T>+u-s>i/=S7G5Yml>-Sb76UZ2.lsNZ'78A,bfTJoGwK&npq4V1@s@BTLT@9W%mo,Dut]r.ih<nfqx@EJi4p1Lh:>D1rgJ5eS**dCF@bcxS$Nc@Om0/Ei-9<*B@A:x$[6GJCZwQq7U5R"
		"/asRRw.1t:0207;QK.VXbd)DqVh_wDE6^$EvZ)4oL/WUEWaZp?QO3Xlodk,+kB/O$nuu8>3D;6_KbNsDTv-5t7RZTotC]M4?WHp9Y3mAfK,+ZpXohLEV0@sq2ao[m,j&)@^Hwu?vk3@D"
		"U.mtLF'@Y6EwC.97:G-o-P+2uqPirTWj'Q*N'S:1ZZZ1]b_91WS5f:]d4t0kSScRp0c^5n6Gx&L&v/r@_9ovZAd'Kb:3aXPe0&mkK3iEaYQIK]^3L[eV<jSlX5Gb',S1gm.#$Bs'mV1<"
		"+,#nK6SqZfi:Pn)C'7QJAu?q02g2nY$M*g>6w7CZ*.Vl@QM@M?<GlYKo22.gk<8rZtg?Jc]@=$G[soUsJpOwfL@FfN`DN;q67#sBb2FlF_8=OTNmV),%B7Y6$P&'_[P59tHQP]gf<>Ni"
		"WW>Ft;^pm@oJ=14P:pOWf::#$+P]%Vf^KXu_@E0mr5$S/vsV8uAkg^gxxMVHk)I2Ipbj+I5migUYtJ*hI?]quZlDSNe3/kYk$Js_RG8`;5u?4D(S.aUX>S%HdO8oI71hSupoPCF$.ah'"
		"QJ,hIQIJ@Xx37:KH%'1A.KF1nE*w4PqI&F1C),R@402JC:<YIL1fSlHOdRg4nc4v'5d]J3VmDfu$hoA;d>K%AwfY@sNl?Gqxs'pQX(%960?hUpV'f)J:t?uc`%l29L*_]>X0n;h10Ljf"
		"o]'TWb]A)d.=l87`T<W+V%8bct;I9&F:i3>G.EI4LO@RU=UV9aC`e+h#+rif)gVjKepm;sjgp9-k8,rZ]B,'I&.&YcT3)=A4q;Mr]QaxU_1H':`[062VTFkQ<Eme1fLKC`*i7a&d?4&q"
		"J#vfK=4ADrS0N@hl.er$Z;A<uON:+(,umo%S'4)HAg6-)Ag6-)[G2?U<,>>#n<$w7<&[;pQquhbiF)FRA)cbC[jhgeM?e.?]lkZ%JD1Hf9tO<T'lx*($(t6MIEEW-Px4=;6u?VoEAE9v"
		"a5Wn_3*`v^*Ad[F2`N?k$Wp=c=0NVfSEd`C2^en@EJ%cj2o^$Fdar.p#q-)]<@Ri$hPAK0&8X7[oHY3%>Bajrgw9m>700U^LAnXY:Vg5^@'q7)h4nOT9:a&#a-&WPOKiV_o>`)7RNn5#"
		"?-w,6WloS7iw_*dJBS7Hns1q3+`#2Bt<rPQD]Q)%9[@&9^?9g1gsg+6ge#fpBPQt<t,a4@4+vp`&LUv$ZJXh?u+hi@uJR.[w2^i:m[RG_d$oI9XJ@6$Nk.J%?_1W'b@k(juxAof89N:5"
		"VDH/Ki5e,#`faj#B*KlRf[.O#X7RQga4>48j>HLH0qIn+%8&=:IY?gWm&Dm//u^4f<B-mQYmqi5C2I#5'K.JL#k86avPn1KT%o3LIEEW-oQ&BHqdQu#p377&vUT.F#####D2&u8LsapL"
		"");

	inline constexpr auto defuser = decodeBase85(
		"=pxq9_8v7&0C/2'K(HH=dfES7dfES715###SG=IGW%###1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##In)##`1oc)l]^0#Wr,Y:vv8]IkaW5;])</+<kKaew^4<54PU.="
		"';Hf0Tj)*dm]ap%$>a3^,6#oLk^@kL$0Ij-Tarx1gIw>$vub4:Dg481aJIVVP(U0]adw429mm&4#EVto`o@sbP;?gc9hWjUsfET&axE/kg_2gurHgP7x<`D(i]b'c_Fsd2RgoWRPl,QD"
		"Us`J]?74Cp'l6`/Yn50`v6/b(qO;(cc*C-<$6>8)I,k8WftupNKF6giH.+o4x+T/rSjRB=9Xir?ldQel[%<V>[N-vl-h29c.+b:0Lww,fJ6#'2Q/%IA11ba.R+8hDJRw=j.1:v-sa(ZP"
		"tnl;`F]EoHl,j;jZ<33G.`T'$J.>'OI*oKg[Q0<nEg-/-KEmtlI9:S:s7Y%^+8r?P/nk>+;^*AVKP$=?S$m]AW:`3c5J5to4nQwBc9+KEZc1([T?dB2RQwa5tlV;lLWFc,4KA*ewNgO&"
		";R0Ed:',R)+9K4v+sm^5*X5]2A1U,K+c^3m`FIemm97T<WdPFLFMkG;hF)_lgRuIY83V-;EYiQCkV`&eZILNm148/`:[>2=:>7=HUShl'unQ#p;GA*Dg<WZ,oLs@BHfg5#HBONc(P,#."
		"V9O;2>)D^AoaoY83+a2sMK#;3t6B8%I],x]J_a3Khm=a<]umX%[K6Mg$3-I.(<tjZC`t]hZ/MVU,uE$/iV.HN.cV6(b.RT%qG;[R)q%r88g+/TN2bQ=WLH:rE@YWfBIl2?&oh+Rc/#^5"
		"hboRf2MS8m8qp*tYKQukxT1q>DAwFX,Pwv3bS)w9I?IJ.hfji8$8,&9=HxJM[IWFUrv)?+oKH[HGQ.oZe_Y*rZ85+`<5+oV1m'C6`Och)YW]?lN&N=ZNZ5j%Mg-q($<PT.iC(rp:B]:,"
		"C8-CkBr<O:aCWJ't]-6j&/)T%FPWXdiqj[*S'eEDp<q$nudQC3a.J@r+9mvGq=gNt?8#NWKLG/ATbj<gS^PBL9`Q&Jlg%i`iRNDngfY9TvS@3Vh`LI8qGGk];^2=VG,%`s^`9`(I'jE-"
		"0&Wg=-]aYPai^-Edv$fb#;Mj?%k'i$/utH%A0SQIdI5[<=)6'YIR`_d5c0*-FP?uuhlgPJl0T]&_lbuEt;jU+n`WoidCGt/Uu>ah6YxhLHx8(/ohm#7Y9V#T#Xum4,V,TNlIJ[Ve%:3["
		"M<8e>xMEjka>EhmZjK61Vpg>]F_[?Tn.%E+A^UThM=86S8b3V::F'Nh:>f1[K&[1Zqp>LmNT_+$ard'@AaQ6=#Q4SdhbsxCZOvS*$Y]o+j;pFXUT3@t#)j^9n?//W/^cX7T-'=0O[c`a"
		"(jZ5XdUp(_%'QM.de%n1Mo.CAZonp'(C91L3CDIel>#D`Ugak[Pui[*bJNS`XxIo#1c5qO'xks;r5/eiNC@6&axKwFBq`o2&Wf(e<'kOPHaGm7AB_'X5Lk#-2#7c=cGWb`qo_rPE0RNB"
		"k3'eY(>xoO*a3?ajYQlUNTekT$<OJ1@]*.SP5T8A2Mw.<OqR529IK*0Sfa#29`$B;c]?e>fERMU/p7vKr>h4L'[wS:]`2>t?e^OR;0rv9WZGpq/KafXuo+$Q_<CT7V1mdi>8.'/BBEH;"
		"WasmtVdjWOGEQn0:>BCdHC'EG82/MoD&l$&i5)Jo=H-e3R:?Y@E9Xf_u.sfP#R/$?]=gM14gdP1#ZiJAu7nY7/AS`or/GCn@4@am+==0giYOD5i(QwMCmGZsSTB_5O&8Tb<KIcf:Fat5"
		"]uIY9Lc;Zjf`ECdrE9(Eb&qN'jxs=,56K?HYXq7Ef@AGC7Zq)G<Ke?_a0Y6?/vv_i%SQvDH`I9rI*_?mx,*7q8dlc+-+hm$5I8%K;x8[+Bp=e9)g<,'>>9r],L;0#k[s6n>KcVH)T$AO"
		"f908ki`]_ulJxMDKhxc]r@3#M.md.D0Dv1Ih$H4#m]suIDqx^.n^t=U=FPVZZH4Ubo8EtCO[&npU[kY*HI(nA#4V)vbE$e#2&tr1(Ba&2[igr.;xv5[2;.62B1hc95Mh<Kajf5V*gG/a"
		"'?4I.+(eNFh8,6kV440$D7DHis0Ed4kBi@UG3*.8VSZerPm)Q@DLx%?L(9g8e*'R3L`#0tW8L@5g3DVZ>_p/Ir,i=<hirLV)$8)K.l#:a,W9>&d?ohE8EIBK=n&eJcRwmTht[[[-dbpe"
		"vP?FK-'T<krUW=YB#h#OiX;H_:#N]NslR-)IO>A@Bx3I'7_m:6?h?\?S*dX,:]P<v;3tBB$qQ(m>hVqjBZ;<T3'?er[fZ`V$D]Y@MQ3:J%Cm_8h&5Sa]%l[vm7))Zm'n<JkAgF2>l5l'Q"
		"OtHE@-^,X6F]QXN@)VsT2j0Y(FDP@M(?Y`:J&32:<#Y,tqsqsKDg265l*UQrPBk/-qvwJ9-mo0=T]>:.ZViN1:vdCb[B3_I)[2r8ATa7%wdPtKZh4o1V8coVn_rDUZq]Nb<a0_)Le$A)"
		":-R@f3le[s38w&JR4Ksc:uS:8`&((DlRZerH[S%?*K1'#EsQZn+IOlXtFWl4d>o]Q[Z=h1rv<ApBf@>_889YmJM3bV3UOu_1J'%1>x_<NBSG->&EwWb0;XR1'NCGnW9F0.^he4:GoXQ_"
		"d[`jUifPf_A00166A<<V#S)a*Rt_sm@^)b%>%M-5>$ElrgD^*I[.'$nB>1%0WLFV'oGvZm_3@j8vmN2[uksFr;Bp>%sM_p&((j?&>K?%TOAfV;(gdFe[&&ulavJmocYxl0w:=Ujuh<fm"
		"jetxVLw6(Kvws/Y&/B%*+t2n<[3YZ$6)8K$6Q^7$bcoJm=LY]fWck)UD:wx/TkqOW%t#N6`mxrF9`f+U0O95U<E(e?0V'e?^.C&=Pr1>avrhxGYth$%:dbBdOMiVsY&^#LDA@/[fP_@$"
		"oA>JVxp@cFOsoJ)w[8rk/Lixdb1<HgNJF$&5+p9(ikmR.mtZAf>D_*R2Gmn:xjbmpqk;Tfs(%(3L<4>&s.]2oERgvRTIT%nl%E&>hN2S/Z;MUW-p5<Ad6MoDPvm8ii?YFt[qQo$MrG1s"
		"x8FhU=Js@kNKP*C@o8)e_YCi@:lO7(lc)eCpSb:tgG1-SaL$gtrv2p(%h4>Td#WqOm_*2GAi89Ip+%hkw#RmaY`eCF2<76nV%c%ru_]30)NJ^0M6YRnk#OO/56n=Bk>C$>5dP1u1ct/P"
		"Y$,f,MSXc?fd_.-O:94bNvmq]=^T[,/J3*Mr;ZLn:9x1ExBjER#)MnObBS-PXLm2MxJPnTGgT;as$-H7M'GkF^C&-?Nt&r*Fm9Hi=STD>X.gL1`'iT@XiC4YfjF-'CMYs4oitf74tA$J"
		"/?=A)AmRD<(tYEm)26>?B*#lOmm4S6_[foaqvTOSqH#RW[ZjpY1d/62CKjG)YjLsl](8Jg3<gbgvQh[8lD$LGMM3D<AJtnSjjlh$tr<@)55vn*aB=,N3S94(wF@l-[3L2dE)&##]Kg99"
		"QaYG8[W'##");

	inline constexpr auto molotov = decodeBase85(
		"=pxq9_8v7&0C/2'K(HH=CD4;-CD4;-15###6YvAJn%###1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#psX]:UT'D?,LRg6A_xO)DR].-,bj,b0l9d)"
		"8x,T-_Q1,Wk>(&,oitQ%tQT&vAD@w)dRL]?i[rRY,PIQq7YUx6gdbR&@P:+<1*fqbWFjHsgrG&<aAS.CEr2FR;[iX?5(j%E<9xjm0`$?j?qJ^+Tt5sUhHPu_@/=b+Xcj^tkCAmI]rbf,"
		"PlP>5a56@TqNMx2U$5_OTxrPaT`K,$19^n<=(cIoONie@C=mCbN;;bqfChLc>P0^ZOo`F@#Y7QdYPJThh%5Klk[-cIb'&W%/$^'&n$xD?<15kI1q9>#.3`uR$ruD*TpWEY(0SqWGewI7"
		"+A(KEC_GSa?uJ6H-kY0,CkNTN+`ip,dp#k_'ebTG`wb1$oRGwdZdJ7Xt3thsd@<o@UH]ZD7$9Tq]$4+seja89o$-ELfvI>_O19>p8ko>Zc?$-VHQ@<Fu)<EZJ%(v5p;=,*1#ZRB2.1dX"
		"akWXj$=a<DO0nKH>L2E*LH&D,nMSq'8I1^R`_UESJ/SIa%iJ[W:4pZLgi2L[P<^/lh'qT53Jkr#T>oY(B7^Zu$Es,d;6e7#mb:J:T%f#[HVq0#"
	);

	inline constexpr auto smoke = decodeBase85(
		"=pxq9_8v7&0C/2'K(HH=CD4;-CD4;-15###6YvAJn%###1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#KCaO:UT'D?KfQ)]7uRg*900aN;uT&'&*7/)"
		"NdE`b$V1q-04/<LX=20BX8G.NBq#E*lf's-LJjUtJH`JQ5(/&fL5c?2%#h=4wB&SIK]kSlqwf]PKW/wWxTOKr5oQ<nJ.9T:Ex7@Y%(NP`,[Kg[mgZ52GX-qgK`2+&RIQDH2`v3kExE)k"
		"KZG^It3QZ2@?MhEZ9/I<sZ?ESCjISPBSA.V7+WLC?_O-af./jp()EXh0(2M&SF:n#Y9f6c7ek17V1M8Oet>xc'n(IkT)GJ9u1>7I4ROA6iO=(m2dQagn<P[^p]S5Q.'_=qiXlCcj/;Y`"
		"Shm/2A#AXBcm+WMC.lPlhT@nFHKiN-NpF<ETofeJDKE7YDNu(tp+bf&I.5P'`*.,Lmn(Fn0phakGio5l,>*##]Kg99QaYG8[W'##"
	);

	inline constexpr auto he = decodeBase85(
		"=pxq9_8v7&0C/2'K(HH=CD4;-CD4;-15###6YvAJn%###1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#J?db:UT'D?+@%06gx$P)6;gC-e'sw8':O'0"
		"1;IqR5/Op/GT_,gr=Rp8=,1YW+4jl]`:;5gV:mrN=`#alv,/Jsl2-[kNG9'h4e;W_CBiDUnu3;IZb-sqa%=&ObC=kTUsO_/J4OW:xw3Z>$I,n*fuTAr:,/A#%X8lp[>dC4k/J^)D^LQ1"
		"p<x9V)D0*f-k%[%9GTQnQnLBZc9@e%vvSv/4hIi_fo,H)UXEO=7Ft39pW8I;/x_lSspfZi2gF.T2&ZN?w7EA*rmrW0UV`SttMm,GjZTt'T0c&(Y^D`=`)]FZ<nVcqkuTu#.*fQNax=+["
		"UfnhU6[&E&rTNn2@;_+O18?Zm4?JB&Q99Vsi^k#P+t78q^Uc3%f2Hi[Kc3m*0H0+:Xo<OE/7aAH4T8G4T:#7+6nVLN[NG%`ULNCZ_@C0RWdZ]L=FmPlb@>-WYw1-+v>k&GHLwsC4]&c'"
		"<lUL)1[nCob^&(]gt0t,^rR5*'x-sgJpubcU]d-&9dhMNYWU?I1MJknq.X1Pk_IqNZ+5-Z7V_^C'DKD5]Rgh.Gb.C%n[G/<Ks'N_G9`Jb%J/hT$qcWURS@pf3=:K)LV3TtI]dW^kHTi7"
		"LsSasO'bAIgjG$#mb:J:T%f#[HVq0#"
	);

	inline constexpr auto flash = decodeBase85(
		"=pxq9_8v7&0C/2'K(HH=CD4;-CD4;-15###6YvAJn%###1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#w?%K:UT'D?job)AR&n),rdKkYLl]u]0r0p7"
		"lvLk)l8C(c4mHF_u7v&Gw-B/q;%M&YNh8d;,+PPXA-?E:A8>Q=@I=-6;T8d]I=9O<u9n53afR0pa6<^atc&+@a[)*Z<]mY1+?3gSsnN[ZU)6)#is8E$NribbM)5Wf9q'>Gwd*PiC7xD("
		"HK4'h51_CTF$VqGHYhx9O-V5:RV^<lLh`-W%]EA6,7E*-4Mn^NHblm5U2tEY@9qSs[OfIWU1V&uw7C>D&u0sf2PVjK)aNuF+6?TOR/PiKchRJZWfboXQG2[20YW_Ljlj9,(#oPYV@^;t"
		"[gr//e5kcNi]0hn&A'^t)NYv3@W^4YMwRJ^D]H`Xw.1gH64m*AR[/';tGCL*%Vv1heR3]TuE&UP<NV8ggRcO+V>?=@''`u<?W>Ln/bFr]T7Qd7&vqLLcWp/>e++P+hl<UI'E<euwLtS?"
		"S<N0EwKpj$KLxTT3GnSc>VMm@q`jo@Pkx&8=XEt5WI&SniXEl?un5/gxdL]N/_SLWJVD021f/Mf7x_wAE#.Ua315*N_):l93VRFC:;+t7cNJ'R@&.sY1be1Q@HLGHi>GQpW/ikt#`',&"
		"rR)s7[nKv3F)7*$uI&mf1-neY]eW)CVpxu7N:HL3NI88%S>d7v4*fD(7PSAWHQ&4u>cq]U<E;,36sPL0qX9inIp=@dtRJl-'@S(b5(K3g/,g62T0?Zr5_Kh/p)3pqC],:@gr@pgCHB`@"
		"s5`aC#(Dcd[vtQF0@R7h;eVu'.7@?Tk0S8$]&v>:sG,mo$N7HP&RUu,w(pW7p<X,n)eQj?Oh,.ps04&(=)@dhCU5j`K4UODs%Tji5rPY[fIKxcjT'M@t)I71sm7X_v&G5oWUct)^rX1A"
		"4`AODulS<$&,>gjJ#xD/rth0G*.?xT_[vM9ds9<gt8IUB.0YIOmqPf,>dv-7dnA9E&s$a<9K-2mpd=pXG8.PZP<hG@1.$pRp%NUJC%p@XB_###]Kg99QaYG8[W'##"
	);

	inline constexpr auto decoy = decodeBase85(
		"=pxq9_8v7&0C/2'K(HH=CD4;-CD4;-15###6YvAJn%###1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#h?][:UT'D?>D<T<gHV+,@%X2t8eO&=cG+G7"
		"WD?r[R'8%IiC/X?H4rq:PKDPiBu#fMq-4x42[a@c$vVqBJ^r6O@CvZI4NmuK&x,%aYb:SpF*0u6$Ygfu_rk56vadiF+[BV6>m2u,NLotg)#P]L+01JB[f,g5RNVXODvT;4q9_dOVCul$"
		">t]pM&(g]kJA&fBS#U.S9_:,&w,N%cuG>:3p#A[IW?RCWYV9v;YVCEYR(LC%4?\?QAh/Pu:DH0J#=P30h>Eo;W+*xwGal5-CXVk`V.$1+:6he5K6-80e;+'Un`Qob)?ugpJ<+UXHGU6M2"
		"P&(W?x;[rjr#:jr[Yb+L`3^iDcI3^Kqq8Jbu*WZMqdT#%<H>kp;?6K,r>9_P#2]Rl`E*97KT.>=aMVE8IQisGjma<ld0UCJb6c:>>4%j`dY=esgF'.]UKd2o2Pk4HgDYdnZ0vY5u0:<g"
		"lSP>gKgOKsw/RMN;`:]lt[T_=VCc_:NgU3<U?1/lbqN.o;:Y13YE*H(cfxs-FUDBOcMOJ@0V+Z4mf5YmbO9*W3+<;oKdUfNq>3;lIr*:Pw(I,$h7g`cD<T0N>1#xK4a.^'+*R(Rx1MiS"
		"#S2,Z$X$oHdHi767di:ZrCO2&O,E#SQL^JH2:]r4WIxcKC2#VFZv/;:p*m16oq&$]X8%o%VIc='8)t&-x5uYIu-Z>nueb588(RO:h&M0/2:'u?P_Yp)B^VS>D]5wTg^CCDk1R-VL=rni"
		"tT1)J##DFI87##-.wdM?`;Z]:p$xg7uMS/3ev(4MjoI]k6k1wCTuDqtc/YKV&(u[EZF<Fpp&+J64`Vp2V-'v)3]gs0]lPUd4fD``cnY)*-CPu/bGq#XNa(-g.q%##]Kg99QaYG8[W'##"
		""
	);
};