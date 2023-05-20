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
	
	inline constexpr auto ak47 = decodeBase85(
	"=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#wA1^:vv8]I=MPb:x/R+,d7Y3@J7ZEF)Z>XV"
    "k3cPBt&0s-Rfc28*kF`&6A't8,:w7X;n-@-iZRFc9X-<(]2s3Y'g:;blla@Zd@DNB/sISK*@[`gA:99<K)HskJsiNmq2X+t*Br;R(msnNEnsnNEnsnNEnsnNEnsnNEnsnNEnsnNIfGV0"
    "V)B$tVH5dk_1&.gB*xBma@/AVO,W?\?/:W/aTVZ50f1g7.g4lPbZg<(^'<$5<R:.IF*#`6[5paZXbNNxQd`aXU^L*`=O$#S=Ci,DdIc.U%NjhSTwCr_o4(2cGT(xj,#s;BjENP/bCsk$V"
    "G8JQC=4E85)Ua.rI@qhH]R8wCw6km0(@TiqeLS;7@VwwR-=nM?]I6h-t#U):^c07<Ew^;aj`53TW<:Jp:n?nqbWZ2@(L*._fxvh/MI7/bV6p#,2f[stVu1/&J.R.`4[>J,6'OL*mc;R?"
    "G(d]CJl[A-M=qU/25q1`uRKKlrb*u'`sX6fMM,j6]b>&Is8d4M+1(:<hnu3?7Q5M,.P]us6&it<^u8]3YiV0Mi>VYtk]koE3&DkWcW,xb0,)$LHg.qJOVHnVurDD#D_Kk5SqJ&U*hD]."
    "$[J]4r0ZD$ta`BqdE#9oTtZ-W?=(eeVw*.VP]tx3W865stT#CaVTUrPP[(1<Gl(3<%KGWn2mu-B9]3@4T=Ie<W*9*ht$@36h9B,7)#fcf6,0r[$76xN9X$iW0:k$aYIm)=ro-+2aMcNW"
    "OmJn1)@C5]-+2u+>db2Ro3<><fT9ie3a`KOisQfkH;E5r74+;ML^6A0$aXb1Koo*(F&Cx>Yn$n_YM%<.Ewu;7po8?3gjAU'<#k'5M.>Et6JrWiv<8OixcY3DdIpfkIvh2Jm)<WNa/1]+"
    "T[hq`PZorH(uNI?sa4Z.'n&2V6:*gpC3o#hR&ZXX#m^9fbOSO_+gA'c`Wa9FEF<kRkcHjQHqD(WrE<bhxcH0E#4_1]L-q,=Y+PO>x<M1*B*4Mbj)BOc8XCo,6dQ.iC,+(0#'PViZB@m,"
    "L^e7@Y%E9Ul>-+(&6)QL1K;(dobQ2$ZO$v+4]4so7qAp>mmxu+fm:mU)miWJi2VkWvq4<6BHe;EmlTRMvkIiu%0MdL&:El`m6u3n$;37q6vB&1BN<eLJ_o%4C;He`nO?mTIEtnNEnsnN"
    "EnsnNEnsnNEnsnNEnsnN]>CUrVJ&kkN3o7&kb&?Z#####D2&u8LsapL"
	);
	inline constexpr auto aug = decodeBase85(
	    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#'d_^:vv8]IS&iZ:9x#,,c(sVD+t'l3WS8o*"
    "xGtKVDHpAOJ4w,O#]#E6;,C@MO2O@;c5udAQY),>SS.,Wq@CLVWaYY_l%W)WJ$wk*G'K4=/b]O[QvYdShcMg^-GqtK&a>nYD1gHh1^VZj^6iu*aGG_c]VVr2Ga5-PA55lBk<hu*aGG_c"
    "Td'A0AAf^'tgw^m7*cjG&v1?55MmrQF_$a,`Pju4$`#%@vHBQ#EeJg.3#)q`uuCfK[eWYqJpfDEUT`-G5<$:S05LaVo:jUF,[_T;o+Ckg&iZ,T7'A=jFu,NaJ]CLeGJBgOSc^F@.>gJ<"
    "KFk837;m*8iE^<;jQ,(3Yt'S`wsP>7;7BM8DT0ofqg<[U7M&P>L$gk#aJd-lh&hw2Ko=5hc^`4UWQl2nJ#$Huaj*F[&.A/%;[o9#IqTRTFutiNHkjbgA6$+&7<2Llt,[,sHDBL8#XIwP"
    "AgEG'YE#HRXk.U:=8>wL?9,92DDC6qI>wpC#mftSPgb6m9=QwsP/[('[>D*iX41a[pC<&UsVXUW]Ge[++0fu34P2t2@XJoIes*q+*fX[-hr4CIf-EV7x31DbI9vjN0uPC,%Fhx8cHF>6"
    "%Vew'dtimcYEg@7W(,7%W736.jS]I+[j0R%*Gk#k=mWw_O;g34FOh7<f7ulBL`^C.;`0D5Q(h7<+rETuxT=Y[mo<P#]DfBACLo(j.$YQJb6Js)x*,%EfG]Bk36a2-9N95E$Y,__2>Q:]"
    "uEc@;.Hs&eS-s3^PuU6q'N_FsxOFP`1LDQkQ*(9+v9sT2vMrt/:>AKoS$hR/9?mDdo)lli:v'U81>%%CnIoXXn:TjF'QMI<V*QJu'^Y-GjCg)3..'xIV0:d:t101r>]0l&rGb+d#$8,b"
    "C%1_@uwQn`;KuW,t:R$Zcr0FD#<l^>l]W:Hiot@.qBt^Y'xkes68OgY9<>A0kt#S&iUF*</pY0Q>)K8+g3iDiDvF>Rg)bpIPIgqW>aRC4]^ec1&fOo;]VOhG7qAuP>HqYf._0INpE^-,"
    "Ku#G=uBPQZQ6xP@%3fmkgd.xbCYJ`':804dtKR3hCPLD=CPPoA'idB/t(DU_EMsp<KqS$21k8c-r[lkU8gfW`LLlvPib;t<l2w7X5]j292]N.m'o8lerr1t9im6h.b:wkcvc.ChP]h%b"
    "hm@Jn,6=)dtkZL8Z$wXt:a;9*MaSp;PV6W1X^eh5cInK/M@SM*MSSecHSK,uq8,_6B+rLoM?h+YoO%'r=ZQo/0p:REtoT6nq@@x8Bh>_tf'ZR4,Od4@sW9;p_`hH^v$I-qZ_gMto&shC"
    "BC]j>:UjGIhfw0VEN5;hneHR4poMUZ8*c.L[=x(T28b#B)dhw&G'xec7-bXB6o8E<-]q#ba/8EZe24KL)'V=k^64amrj/KLBUS@`Tx*>jQq;LU+&7Yl&CGTacr-t`[fRuI5f@#R4I#VY"
    "E]xecvhnqPflI2$nhSIAsd%s6hSDhI%.:lBk<hu*aGG_c]VVr2Ga5-PA55lBk<hu*aGG_chRQUad6HFQ5uwJ4g$###Ub90<d$bsAP$###"
	);
	inline constexpr auto awp = decodeBase85(
	"=pxq9_8v7&0C/2'K(HH=NSi.LNSi.L15###1TX8BD%###1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#B_^a:vv8]IR^$1A,,c-,)93E]M,YZ;_G@B$"
    "-=T<obENin<nM&Yhn^XVgS3pO<rc9=CTI4W(;R.[O.*Jq_0'k%.2^sJc)jEWr?<k:C^-I+JtdrU&a15D2pISifc44-pdNVfku0E1GMarUkM]w)mJ2N:asA$RGb*d.mJ2N:asA$RGb*d."
    "mJ2N:asA$RGb*d.TA.]-$t'BEW_QZ41Yh^[+6Vu>)'P,[m82,ucttB0nk4%oR1;Rt0)1--^t*o0X'4PnVbR*V:Ot1LX2filrZhtmY3ERo^*vO_6no^9[b(@9e;x*V=Ylr9`BxM.IWfP1"
    "pLU@%uJ/gTru99iIN7Z0P32>nZ,V*`raGW5BV3YueNN*v4F4Y^tkgB:wl0Lmb1g8c@bGW51l.aN@pch(bgD4t<YZqSH0tm*:`hp;2rZWbq0f43HR*,esv)hq<s,bKJrR5rEEQ4hE6SY5"
    "sFjRN9`%.)K[ehc=4P%l*+Yx/NYf1=0h*e84.F#hCTIjC'DlC<#3W5tGe9%d-)1+L]muG_0p'ts)$=krAG9)#Y&VJO&fWTcZdxj(UNR@W$EBbqbD)qP8jfB09Ivh41*ZP7M&N;LKJ-<0"
    "h@i03+KHI,Cx^JY/>f&%qEA#<bCxqVFO,5I@+IrcP4h2Tu_Ya7+$$@Ev$q[J)/t#[XjPv7O3D]l2<wL]JKshd7[i?5.n,T63HIJ?$IlB3Ycb.dxFdjt_mJ19m?<&^Bu-H/72.WJ*K/#6"
    "S<9-Ij-Jgm3v&;mHB3WuLYf:ZhG]1Uv_6PYZD-V$X=$nIe9DtTi9iWXEW4pBfHkG]*h]3,W,S+TKg%@<CSMC/v7l(*[nM-Vr`(jU,flaAuH@Yq?wnMeY.&W11EB%XhW>eX=@pa0H[.O@"
    ".tTmU$Tu+BlV`V#Tu%H]kA;-/=H9Ik4Hf%dYWOlo`)cu]3KK_rdQ/,*U1:d?1F^$SG*k(Jmm#0UsZ*x^8e&,C`H+krdFM_,S[B&(iVgeqrPC*Pviq*MA6v)I`q=bn5->Z1=YgHiU1w'*"
    "W@[u05Uhqd-9K4D)5-*1,XErIHR+Nq$o3LL86h1$DTJEaZ,C.R0v/.@iitH/[$@0qrJ<AmVA4bl'R<VM;%1O?PA@JgoW]2[Ux;ai21Timm*Wm2Tt#?hb,P=,cLT1&jZ/Xg;=elNwl[dX"
    "?7oiIYd%kO)4)A;asA$RGb*d.mJ2N:asA$RGb*d.mJ2N:asA$RGb*d.mJ2N:v/uH=So_o1YEuOA`xUv33$###Ub90<d$bsAP$###"
	);
	inline constexpr auto bizon = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#:)UM:vv8]IftQH@Rc#,,BqNbbKaCvC$9Qm'"
    "H%P7S-</ORO^'esb3-t=W$`HbmD.f/JVUpRtq2N(92cg)DmEZ-AdXh8wB/$gt5`HO&JZht%dh9,S*P3sL9RkH;.>=lR?@ne<Xeu5kAvcT09hu*aGG_c]VVr2Ga5-PA55lBk<hu*aGG_c"
    "]VVr2]6N_Ok`5O7tZ8u*6I2tBXrn%,Lh2dch-M,YelnHMO$0?c],(5h<V=$tE4DHPn_EuS6*CcJkb[T(X[.EG1J1(*u-fEc8/N4uod],`aSZwF9K.7BRAw..3g9^)SlNsQcI8%_$9Y5e"
    "q%R@rertPD3Ks:f_8Kf3Ldx&t-+;U#OL_[d*ZS@>TK0vWv?t$KRG4TTvMb$MaJ&I(4T35O/;/4Qh0*.D%tdmm73LF;m#i62MfoMQ7tn/(Oh]$%w_HAGdIncf/M)NqJ9_^N`35]3kk3>m"
    "b+fo7S#wb+YFsQ00e@jAYd3qa@`lP0Vv)ihv[YsolL`<El7P)+NR`e4C=stc0^@SX>?c$KLe>r'kpZ]&6d^7YUU0_'sO]bjNSs'.B7p@mB8X;AK.oA*ho.o]#qHA&RKRtSc(.K6rAg.W"
    "YtFF'?jXq,AnL*('iY3G=Zf^@;(Ov&SwGi'J@UslXlLw50A3wt9@WPirfFdIL4a*neT/Z[D_5F(;)E^t)^U6q-wn#mw4K`&S=a9p$G7`>[?`tg`<1?tohLVmQ#H#O&c2Vs9_6i&<eR'Q"
    "U:>fak4,l1PJZFuvVCFA9N-j<f:s=:5h96SgF#s<@)HM8)Ztl.gAqOGp/CH?0UwcCNs'*fm@:MV'Lja&KV$>CLp^P.&@(8-5qP8N]G1ST7[?fAU7IO4xU0j).4WtVl%K#k[BK[T_t`pL"
    "^3HtZ=XEb[^rNc)JA@8$7<6c@>bd`scT9LrL/cjKoX&gRMd?=ZPCk]t=2NB3,acuQN`If+s#USF33JLj5xZ>Hkqgbr6WWtZ>uV0%<?OL]MT[5Q*Hr7HF2N[&3j#fZD<hi1)`ERJnO+j7"
    "&*5iV29S/HoBF_^Kk+'5%parD5:?3>xc(?qtZ&udDrEdn:HS:O7-K]&pn*o.jYti4k$[:BRNGpTKD.Tf=Be'f':84T.wu/is#?X7$O@8M(kbK+ZbwA<&]-V;+ZSBp>aS5Cu8^jH#%[P["
    "Te@AA&e9%DL4<KUA-DhlCWvLf;=33qnSJDh<<+8OG(PKL+k5B]m']mDtHxYZi/ZV:vNgVosM$$oW(J^b^ZQt41;eZmi<Pn1Clp2UYqeb2H(r'vj;nBWFOV>nFJG_c]VVr2Ga5-PA55lB"
    "k<hu*aGG_c]VVr2Ga5-Pbpqq<rp<IBw?984/u&##]Kg99QaYG8[W'##"
	);
	inline constexpr auto cz75a = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#NPpX:vv8]I%w*[j$T=-,oDae')W%<f3tDtd"
    ":mg(Dj?16:q^4-=$$HlKZphk1V#7N:`wJCmGZ7R9iD]K))eT[@h$wM5OB(p`0xR;sJLJhF>Y>3sHK#okSC;XEt$q?k8R2TkV[7L(hV7L(hV7L(hV7L(hV7L(hV7L(ca(GCk'AnT#^*LZ"
    "(lV3M18R*EnJc:,Sw9A<9b&gloQM-MO.r-/]6EqEl<_LZP'8#G]wE02DBG&U&'(Gh%CPQ3cs^802Xbp(EZNOH3gptR:8EPuxVhC#,Zbp(R`jlSnFLGn4Drl5tUI:Pw/o#5EKF[Lktm#e"
    "E-f7Vf7KKr&R62U_jl`:,e:SUG/u`Z)3h<AB%Ac,]8F%Nv;qhuDscHH'Ks'6sQY=jbGg;dim[bSAo+Z?_HZ8@v9hXhXm)'E9@$T]v6NrEB7*53Yi4l+LJIIS*$(MEljDING#B;u0uM5-"
    "ZC%SgYtCO`q)8L]o.M/-m<3G-*dmGWIi@Ge%e6-@WdBL,c5MI0XSMkWH;#_V/hwDQ6KZ_;3V2(nC%AO^A40%/Jpe'18^>`JOZ*qT_;&u=QikvGmW-o]`ux`9Ci,LtK>hZ2E#fM%&*?6]"
    "`W+:=&P)aX^MFF-[8n%tnfIeB%oK1W^aMZcn=BHa$`ZBYU#7>EpdEE^fcmi/>[?Vgm_K9,:[=*Z#^/m?7Yj/Iun(8WoYP@kEG#tc4`P7[fdral`97obWW%*N8Z#qEo/wcf22=,6m>R9X"
    "R[9@lSWsF_EUD2awMj$DarJ/tMp-,[VYwYQcr&&56o:rFG'I+BIN;a[`w=E2Dv9+/bZ)`%B_GoH+ARd?:?`D'=uD$<&Rw/et@^W'TphU>]0cooQ=]Kjg5v3FeRAsMON^L40GtcN^,_w)"
    "6P'Fq^1LcF2FXQ/+h:N7wd9wI*85^PQd_x?d7ro:xn]/<^Md_Z05T[@&K1,AkN^p[TROVpT=)2c.O[T+H`I?W$<-p/p_78:WPM?R0b#ell;hF_HetgWdMBh%sw5JYf8A60.Vq*O:40RA"
    "CvDISRS_GL;.6YdSK4U^>/rER@<CA)(wwjHVAPxZfx[X/&]IjHbiNQV7B;h'wj9jWZk'CI]THpG46@/;s*Rv-V5Lv-V5Lv-V5Lv-V5Lv-V5Lv-#9se3x-]u#$p*G[.jniY#####D2&u8"
    "LsapL"
	);
	inline constexpr auto deagle = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#W5&d:vv8]IBxq$<wS=-,OT&)P;A]vQ/4OD^"
    "rS`=8Wv<sM<p:fZoZr]R:G;.E:/N6])B=n09+[p:;Tg5Be1bj@[Q$gu?L>F3>fBZ3$w#]<CkL>n+1UGKag=$_lJ########################k[wE:xVtlP1,>4AhMl8ORrS#gvlb="
    "mxwV=G'N'@JAoAE^FP>pn5$&s-bv5oTL&44gg1=Fn0<a8Znax[/^PJ'/s*-[->W8O(s8p)9SawMf^T,s.St*awcmh[c/_`ZV)1jcHV>,=EUUf)DxiEc-*Tq<A7*cBUpY>FNrWW4H'RfG"
    "?x=XQTb).d+H+bgEk2x_V_>/QiND@:RDJMCfP(M?<W6E[D]VkaxnXk*9o*<iRY$Y7nG4.eVtRs)n`/-kFZ$Y7UjO<jd6/tj.f[>+=_Y+.b@EOqd#Kxu#k04[R0/HpS#sNs58DLKBrI_3"
    "V^8M@0jV+*Wq=ZO(M6.`BLHM?Z&*lIK5=tub0*D/q-w(>l>SH)ePd'($Jh&DVKs^TDw]G*:[BB>0c8vRBliL'=+;SDT=]B>tT%ZNBQ+>q&$(SAJi2YS$.9<rw5twI]'@k3VPEe0No`1^"
    "uofC,GFY+<E`1JBVB-jY$2H&*N*%UuZ5i4,sMZELfq)Y;Mqi>IE]/SI/'gtK7<]Au?G#+p7:*?ctY8NGI=WMZ/=o,sLNd]giodWk@=RC]M<5LjW&H`mkn`mDZqO8a@9#/oV+hU10nZp>"
    "*`tMCL746[P*?HiEj9Vjda-O:m;i7T41E51sc,[Evv$V$*q0Xbgr,ok<x9F=LGR?i9nA/dKpeIYq6gXE$&LJPHHE1<gm&)$#JgTok#:C;_b^l5>Z$DtO_'X=Nr?aBl-nYi.._flA`xG>"
    "/8LdS0X9AKdx1H[t($YK-bEDHUDF,8####################LVZ[Dl.V'c3rJCs5AH6#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto elite = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#lU(f:vv8]Ia1L[Er`#,,pdr/4evbu]Srtc%"
    "<6.=(s''&YOgehs`]MMq9]0q`>m#d;INBKWwVJs`f35*=xhct$QM0COPO;23DB>CC^/#*n((Pg89Eo6DwB(mk)QnN5f0vN6IC[GTAQLv-V5Lv-V5Lv-V5Lv-V5Lv-V5Lv-V5Lv-L_k`Z"
    "<x9g/VsJ4iVI?mQmjVd:xSb%]OlV8F2S_ZdM*NC5G2+/dLW3@5V^crKK&Ak[;N)w6c_@qc_rOMNwmuPq02x67E_VfJRASfiUJl>fdE4b_xFpSBI+=hcaIjJ#l1sHS=+APN^KiL[WHv.,"
    "4*JS/Z8INEU%qeq_GY@T:ekE;sva^A)eR`S8fvN]Y0oJJ`g='qHUW&^W.R%Q+s;T^D3X12K/cK#P@Ze1s.;';[#ADhb?xRUF<[22tZR:buINX.(uTj-wf]s*&IrZt1.LMHeRdg?Tqa9)"
    "V_j$D5rVW=1/T:`x#h:5r`MV.j>Ao$fdnD1]QZncxtuFpA2_`+YrT-`x:?(o[3)?/2fx=C]rhdS:j1$]CXb/$YZ]C^F;1eNv_A[B-K]C+wEYT^6r,FSDVW<:9S:/Xs1nY8O1GGXxAYHT"
    "@<7ddvSb15<F3Pt5Q4>1Es7iBSYRci@av^B9@&d,4lSUY$+7p).2ap38-PSkR'6XdCTVKHpp^m.p;+?,A[gk8V34a9+_Ek4nH3M3dZFwT@kJp1xUq3aplmHU=viwG,FD_r/QWB/%8=%*"
    "oW-5J9+').%&:'Csvo-j-$cT`m^E-r[sgW',,ON_jEfk^Wqe8-+o=t-')nM;E=Mqg_L?d-`ugaGBdm7k(uUY-5UJakX&;qcN;[gOA<:LR6c.84vo1sj5-Y7<oA7%0nF(<N7Fb2v;>1(["
    "?pN3VU[ZWFuNguBeIxI^if$(U]cm/G,497V62cJI<balOdtf4XrcX.[N`sBRdG#;=pZTPkC0VWts>X)<NX>tecf?h&<v7IQX`:B_u$iG%hagspdHe[6mlrsW[qHACRuuc:l((2XT#:3K"
    "sJ>9GL@^N<J=pVs1/f,dY6WPY=wDb&@=6,T0?Qv`LFSd`*FG-r#fG37b@?nUpOodSPG7d?P1S=c&7AcAq,(tIPQlLAO_t*[[f(&`S9A$qd(d6R9xD<'9YDNaK''O]sMN4suvfh%Uf0cK"
    "+sYtunxQXI$wQF+Uc&v)Ne5`dLN?6va,:;jrr5n..oSBk<dUW8Sl/&Wo#QGlfUE$0_S4/l%BnwoH./7kVx.t84Hvs84Hvs84Hvs84Hvs84Hvs84Hvs8k#a4>NN8VKRX7V:*T*##]Kg99"
    "QaYG8[W'##"
	);
	inline constexpr auto famas = decodeBase85(
	"=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#ZD8d:vv8]I(9k<k)qf-#D?#T0#?Z0k)k*:8"
    "m&(j&.>xv75vXxKbDF7A(o<>pQ%rj<M/-oNA#HsfgGM2=XwdhO[tocN^hV<^20*,])9gvOwnNneeMo%6'a:4IvTkAfI@,mU1H?GUsk_BTo4(d^4L2BT4L2BT4L2BT4L2BT4L2BT4L2BT"
    "D4q0Xs@Z4OFIJ/005^_Q=8(7T+?8VUv;`*o6WAM;JJ`%12v0GCAr?#//$HtR,go7_Y(DA+@n-A3BuA8Y$UJW48BfI=ZFbhH(rYR/M7:gsvs[/5qmw2jQ*sYZov[]T0:lrJc5)f+R_^Cc"
    "9#_1sDk?CHZDfe,RAm$)]d$&e]x3DXF0gm5nVEsUs)#7D+$OA'RTcQoa>e;8GVJwoXplu2W%`.eBv(4KkbXBE)7]r.3.^DPJV>6Vi]tQR2AXfbM97.o(/`A*Qu:D7/<ssn/wr.:_^e^B"
    "[02s;(T$lO[ER(td91_TM$Tvlb=r4FhpO&^>dD'mG8ZY+JBm_SoL41E.:P%K6,^_1:)q&.JOmBG&,]$Jgx(HkBK.]BbjO=0%uK@k/<x97l$hp];N`LP@1>n?sg#(KV;Kl>_;1s6dl#1E"
    "6O.hk-nGt,hcC$bj]x)Fj]2M4b6^:pwZK1=/#k]p8&<Pddt,]qcuc3n9o%%W2,fC^.o%j%u]g@:In308cMr0?0Ld8_+3uppf>fJj37S5^.Vh&$(_?c&FO5g_-a?;gFe>NmHeg$DFpjR?"
    "m&M?+-NG:t9QBNUA]LrtaE/?07F'`WA#:8j2Oi'4j[$MXp(D#j,t_sK8=QSR[<,Xeb^PXruhb4h9$bgj@4Bt>+wmbg03t+:d_%-?M553W(>-OH;>acn.e(_fb>CbSQgbsrnxoT#q&B^K"
    "+$;s%jJ9_(&@`pB<^vF[9f,GkPjD-PG60a>'Y.F=G`Pj-Z/-]`lQgGCkpIiq[uwcnJ?uT^qF9J*Mw]Jm`,V0R?5EWgxI4Al)MrWG:Xpn7-][#.^IHcefg-5vjSMhi:Ht-+?Zli>9AAlP"
    "WGcFSOI<bMmh,uQFn*DC9%n/JV@4hbn<tsgU2'`@4LawFD3ip@1l9?.OiLSQm5?<TaeTs;tm[c<HJ#^6`_,Jo:Jt_Z-?<.@+t'*R*0Q,T0*KmD#b=-q-'uOKws-$<O3x8#Id_x/:cB;&"
    "%Y?8d&@li&m0^DTv?3`)bILj68_XI5Y0_TrBrwSgt8@X8?@Abu]3[S$d7L[W$?3BT4L2BT4L2BT4L2BT4L2BT4L2BTixd)fb3XCINDeY]n'=XEg####Ub90<d$bsAP$###"
	);
	inline constexpr auto fiveseven = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#,1CU:vv8]I2<in6^S%P)JtHT(cT2L()^*VV"
    "R(nD4*veMQvbNl0N>^9$gSS6b8q6MZW.g0(ea16(r=68(v:xF48bqRAr;)Bfv;2B+qV+hJEw'/(j6K<`ISkZ_5,)KIN.#######################x#H,M#1:JTZQ=d>2r899/RPd0"
    "AEQFIaR/7@PG=bV:FmJ[+o6dR:r?$DPiEBhjq+(pCT4wtDWML<Nl+P]m'C_r:bS3%F]2=9*l=w:;s?[G>nX)#*V1?5EAEprAlt7#_JIgThinOOM,K:A-eJZENT:X$Rdc$*^dZZ)L85=["
    "T-S9oQ<$iHfDo.q8kU`5dxIHZptWFLta6&SL8hCqa%300h-WihfWX;j<G+.:;RXPB8MmZVdASf?b-F$k^<tXP^u`Bcj4_d+kJE).Zi%.Eg^O^'Ks<rQ8k[c>8n'WObYVehmqW%)k_;<*"
    "GUcLk[qNL^IUw#/Hwdjm8*Wt],DRZVc^7oKh+6dE`+lRo]2#-^nK0wI^]dE4i9@lin6A4fv<4kGCo_>)qGjCmq&gS4J3cXOuG5S>=_#H;D'38h&LMYjq,]%#r@QMbG*kD]L/?;0,v$Ns"
    "c@.Xsll/JOvADe/M#7E[FWRiLB`a`1pDVVY@0u6ePoWSTGktEH?Rn*Yt'*RJ+wC1nj*1Zq`mpL#&%NqcwphWOWiP;A-+D02ChOT(>@S6#.%F/D#*T)420m]8Ut-VFtSvaiVP@CVZw$SI"
    "W+GNaL'JG#####################A,1/>b$$Ii7f[7S%KO.#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto g3sg1 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#ORjO:vv8]IftQH@gqD*#W5>12t6T&^Qd8V<"
    "AqOgCqc[Z(fNr<8eJN2OPg18JL'W6K7])YN_ZkJ9$,RUV.*[)'/<v,g;S*J^%#PDJH:Jkfih2n^pb:OA5(qob__Zi@S+q#D^:xI3'+fr*aGG_c]VVr2Ga5-PA55lBk<hu*aGG_c]VVr2"
    "]shQ;FUk`ZJSR15/99L0;#w]%)#fsLrP@bs_[fe]%<pOnUY?k/>h>bIiA_Ip6hGUV?A,$tO%MV0:k7*YXUx.#g9EIqa+Vl92md0_c*9coj0Y8mV0k][Q2%D>xc%jS=#C1UeHd^IHU98W"
    "UPj%K$45?&&.$Ino/l]Oek0Y,om7D/R_$82VrrlR70H^qfttvcsA'VES)vOTEH6X'$OTdM(%B%BCcb:u_)UJ]1B:#lRnZkKb_C4&9[PZ<]w[IuxpNHDFfh%Yp1*r,P+./Ik^f-jMfFr0"
    "2ifFUo.[6fn7.D7^P*(6ED4R9KviamaP2&Y86Y+/e,MV&E)[1T3lhd4UHBL&@=4-P%kioiHhGg*$+YQqu9XU#M>jahaHN%`J_Qk-xE1KJ?lU)Z^_6*;B9xC]gg6Sq/e5<a8uuSL%EtRC"
    "Gl&6jR#/Z6#a=OY&f)Vk6skRmLS#]^T>p5Q2#v<o*WIB'Esj:969;vsY1Vb91Tt#-N2'5&6`*gN_?$BE$k4-.oKq&9-E[-U7NF60Xd7FR624`^<C;6s8Uw4r^B2>e/'%_#8G.X/e'wX;"
    "iwppBhj1c_YNdU4lcGC>J>3rYmU&JBdt=a<0J#C6?;2vaQwY`ZxWi9A*OTVc;hg8:WE*v33w[asd^0R;X6_p=OFVLb`Kg1[siqbo1ex3rwELLj#_O+jU%f(k[iNxGXl_U#TY4n3rqDKj"
    "`r;f9;Y<xnh+&br]2%O;Z?3ZZ9_oq<-IbX5(7_m<3xhaEt.fP5H<W/&/^ODjFWc$M0^:D5$hP,cv>LY'ml%Q$Kqp>[NOl&j,B6uh9k1.-gO*Heq&hNf'x;Y_M8QSP^vJYf>Qa0=V93lG"
    "Z`kjc1_3nXF3x-tee[k3vWo3Qg'$F`u%mYl;tet0'-LgO*+q=HZ)wh)v^%=qEW7Xc]r,(Qsk7k1IMjo'>a<fG.u5dcQa7_'@;o7P)?%PCt_$6>&+N,eO7*N$e)%i6hC3@awr4/<#q2Z8"
    "GgHop%s>P7VU<Up&0TvIN0?ZM,;fQ66QP[;5i6B+g/jwXB9eTrNV,S?ru]Pd?%1qD%no[:o`isuI[j5<GhOfVwEH/b4F%T=x'bV'kXd_(LkF-gYK4URNi$b)ren6=X2(7nt60bD4]Xl("
    "&BIsg;t+.PA55lBk<hu*aGG_c]VVr2Ga5-PA55lBk<hu*<Wt-W]%C1L?+HR6ra+;#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto galilar = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#cu+e:vv8]IPKCO@mBe-,Wvg93lkO,*==,0;"
    "Pk4e=2)63$h#kgns?gRJfe#40KMwQke>x5hQbVk>FLfsfpV@KHMM:m4I1jJl7(KYIKGK24%d-Lnt;1gpxUg36<s1Nfg[34QvofTsIK/p6?7JCa&5HqChZIqChZIqChZIqChZIqChZIqC"
    "hZIqCqEfo1_SQ`fH,-`TO3Kxp@c(>Ox7h#pvcr`X,/*J^#Pxt6eei^U3=7`^)vd$9v1mf:?JSuHMj8(S^HCLT#@h@QPhp[GKP&<.^_#0RHR-b0ug,]G87FtR_mEl;$IhO[WPPE`D3Y(i"
    "x=X[qblm(DhD-G>Wl#/qOlp'gWMGK6W%<Ttk=/E+D5g:t5fT?JW+bx);>uMdo98*W,HR>.U[oH>60s_V-/'$UBWkn6hpYj3<_crZ%,ZQi;vd[sF5+ZE4m/VQYfVfnXF_O_/JJAP3ME``"
    "4a;5hpYb-@?3ejtQINi6V)b[nuW<'pQ.=8IU5*tl_9n>EBr2gJmPc]['ZSLs1,3Nndn&qq7vV;N'?QT_r>Tb]B9s@>d2$'.1l_U+X8#>CY++7ZJkOvR)m9g0?=AwOZEj:EXiFma,fdE5"
    "qBdfTK`k`P+MCIXDA>`Ga&g+e<^J_GN^6`YOH;:Ik_xJGT9pf&Z2CN6V<$sO,2`Wui`[;a4*9fJ-ZZuNrP>f%f*Z?mmn#2ZT(vb`TXVT&%>$gq`%w>ZUF=w8=61%XL>eMn37'Q+s4a`M"
    "=$($`q0uv3d_o7gX=vbi[Bah8I]d[m]vLMht5=ddWVcgO),uQY]PvVhI%k=)D`d`)#?:CQ#s.P_3jvH^/MNIk9Q'iX2SO9>@u;&7Cm;cOXpk*V7,VOX,I7e,<1qI:-s`/up..aAAWdUX"
    "sew9M.sVh25gAn.V2)VA+[e#0Vpdt9vHC=s)NYSU,<C?o3<o[m6vH<Qti.NoR,DLYl29)C:vBdo_4iPTFXYkHTX)aic(M(J#Y`D:Z7ej,HHW1m6MSm)uUlt;SAdKJ&m0>^vD^BANq/77"
    "tO+*.lG6A;*+VksT3x4pe.j[42-K:9rD?x)4[j8_[f2VX9_Y>2*npb/1vssfkw5`#mdw-W5BBMe12eWSk543OoTjdIMhC.Zp5hDR@u'MWTN,-:+D`C/H3#Vne,=6[e?:())sf+5Q_mdE"
    "*)Eqh<ETt,[n`ES?L6j/Q#`15a'1T0qd&GQ6h#KK4$h$Lrh$$b0:>U'J6h?@>L$H/aM1su/X1BT4L2BT4L2BT4L2BT4L2BT4L2BT=Gts;Rj70v,x>/WBXm2+I$###Ub90<d$bsAP$###"
    ""
	);
	inline constexpr auto glock = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#e,;R:vv8]I16`n6^S%P)L0*6)_,^ts`#>P-"
    ">E*B0n&`;c$qZPKv..0@[)P`fIkfvNXZWqMZplKPIOvgE7Qfq*7T>;Rvb+W&CHUVkuqq>kvY.RN1a6d]NH$jeNrJ/#####################R)V7W?pdSC[#+j8^fcc_;Ocmgx^GGt"
    "AaJ$-s$a#?J@;PEe>CCR.'B6eCe8I<cqV[SS,ed`QCP9ih3,?L.@>^3T73@f1KFGX:exEAj#8YEq[Bha+Gw,RI0mOR_(lo`[,=k`J>stdR2BDA*#p%7G/N9k?7_ghFO1a*Kdf-eOXpqe"
    "JbZf[uB5S_U5l%ZN7?V+hgMn?'F5;^$ouDK,7[9vwJU'&moU5],+b7K'q6OKG:mJ,7LXXC6C)R?%kqcrHP60O^T6<A41V?Luhqcr1w-H1_hj#F;L];Z6d+QJ[fBmF,jc/J?@=^ORW#ls"
    "1T<%[YEK+vj?Q5,8/SVC^FxXi/J=Y6[$d0fiXk]h@l7l>jQ]3)B&+(]N,8XlX9%V`&lB/[V'a^dK,AQ`FeD+SONBk%vhk.>wvOv%mUm?a]T>k9`[j(ME29cT%L/3hC(?JK?9Vc4#DvkM"
    "]*&LrJ8Z,RLKQ74[U*cC[`<mut.HhA;r+t4`jF>VHOCBV#0UIPU1eGJ#I),UQqMnu/(W?98q<.e#`u&2pjMf<FUIhZ0(PQ86MO'lQ`(iAv>#k]Ag*######################kP'0o"
    "=KU#;(82,pX1*##]Kg99QaYG8[W'##"
	);
	inline constexpr auto m249 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#@M6N:vv8]IU8I<;Uv%,,?s?h#71nr[nEF1:"
    "eg4%0Wq_UbM^cn3MN_DXWrDQLt`L[c=m5eZJjFe<BZS88UWg&=cpss&A'x@.D&gvdn'OL6M^<jmEMFDc&UE66]xoAT_oApqP99Mf3J*R7q%Rup(65lBk<hu*aGG_c]VVr2Ga5-PA55lB"
    "k<hu*aGG_cDC9RrUud`&:avT%W@?Z'1MucV+>mSn:>qBMt;0_$.mOe%U@1`M(3%:`a$73o*='+D`PsGCePxfV_jpJ65h%RE8Hrel#`%VJ+8HZ5O>^Nc;>tD*eeT4MD*'rKpbDKh6.xdo"
    "69Qjh8Lk)Se[nMiL&t9'PRD[T_DChYBopX46^Gr)1cidNJ=D&9n,x*a_vfa-;`Zvdt[ev>[=XYs]1,&Dxr7#t)EA.oSVv@AAg,Rc:#ro3,,gk8pHF7d<g>([j3Ts3smA6*lZueAc>M98"
    "xi8b>(HL,Pm+q:U'XDI%HA^%?Q#P(YbY>Lak96:L5B,Km7%:k>1ss+[$#99<HS]-Wu85K=4qDr]i6vmdGX4MY]g%$*V?DF9lU%:YD]Umjk/`E8K7W/O'cmQFrqr=4galv)CxSP6fOYe5"
    "53]6A53acVo6Xv?Go9%Di9#mW].LGH*Mg/L:U(WbQ(j1H#[mosp4A-F4Vjw+GX3>4[4R?&;Y:?Lw+Z.$sj/e=`'<:)*m[*HH'E$qPJ**eGgmFrnDaLG_gUC']X%x4FNNQ*6o)=gUEZl5"
    "P5wq%:+3K]Q+8GDl=H)EX)0x>kKfGNu26<XrsAqW1SuO#gu<C23s)L];T(=bkP9Q1$EodhIK8cW$wL9Vlcw;0Ot?Tl.V%'FLLASa-da97m5@i/%^_huV+ZHN[`6Jm=YJ8dnSXvd>)[_9"
    "1p;rQB1[2M(YemTvv64kI>w335TW6@26<+0N]PMu14m+8F;;RXYA8`f(FAf(mIfo*FIS*oH4fO%gF^eLj-3d0=J6-[kIaCJvISwP5h*4$d^)?Gn:vx,6@f$NxI(OllG4L;r_M*F''0S+"
    "^.`GqPUa0p@eOJ?Q]olWS=H),SDdKjM_X42jTEX0;n)126xC.B>u:g;VV<S-r>=o_:n5EncI5FMK=s%KaA8=CSq<WN?qfZ5_1'wk$@)<'Cg/s/D2L/6i?2ElT<Q0jE;[VnvIK9-k4q2O"
    "U_($`]=YEsr/D.YTd[Jj8Vx50YxA#?P`[%A$ZDG2:Adcnv>t0hOCICO0$i9.;W#D0#_d(C2@PfPn[R-bD(X0_PR%om,2PW69,n6W.]gnMfl^O-<.:QD(?Fago&#J0E@K_[k<hu*aGG_c"
    "]VVr2Ga5-PA55lBk<hu*aGG_c>`jEu_$O7$N&q*2GNAi;#####D2&u8LsapL"
	);
	inline constexpr auto m4a1 = decodeBase85(
	"=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#xB%K:vv8]Ii<aaAbS0,#PL<%U2uQKDJtX>b"
    ";[Fp<sfYa%3:C&;KP9a[3uDt63>v?g?Hi&=Up[M-E:WmNas:b3Yx9i<bMprM5TQ^TkfqQj]k+jtdV&m@rfS@kcv;C`@U84-i;=%kY2MAfW*OYKh.0lBk<hu*aGG_c]VVr2Ga5-PA55lB"
    "k<hu*aGG_c:%wX_(b6i[G^G?t,%lB_Yha%*HfojO%u+He,Eb*<cZip#8A+spx7g1HONdb2+c6.1V#+klMv3lUY(>/KNr*?O>tv-nGT>Bts]e4f4qQp+6'l%ISew]oT=4k5Sq?I$k&LD<"
    "'2a$e2L:/lOS:e$?l=:T%r6%GYj*``=Jo1.k1g5k2g(js)paD0+xAnPu-@t+3^oMb,>nG&c@D4jF0_lFEb2/#S#%%&7`id0qf+qaC+=<3/5k[P^(mqkYM0?R^AwRSi_CDLoX@TkAw%n@"
    "]?UI3%J4U%>GUKbaUi*W2K8^g<m@8-`_8U0Ssmd$J)hMMF^o7b)+/#>L[-'&Z)<Gir8[$;opJ5[dM57:tSoeaW[Ra8.0cr,E@N#K/7&cYxg^92bOFWd6ZqCG/-3:*#%#imRAjj)tZaSk"
    "6M,?E&SPqV_<MsfSfMa-1(C0m,2dW2SSL04Qw*fmg)P<R%THvjU544rS(G;lhH51GOX[&&g34k3N95fZq&n^D7O@Q.TsDIRCJ]LP,gPTs%l.(gQs?EF'ms^S.4iD)6bmF>+#itO2f3,e"
    "4:E;EhF3RfQf']f,3XMs8$PK4=Do<UmF/rtUep.cT7QvI^f4nAAZ`%;%@J@@RK/oXsH][7*OJD)d5ZwMvOiU84VL@ij&sTkuemUa#=tVhl/BcmfI<B_VEaeo%eAhs3JlehOgN)drTcc("
    "]k6,CoiaxEt99%b(hWAt9OsOZvmE`HF.7%4FpTKcOC,1]UF-L;TfR<Nn/h*(RP8t2o_*VU[g&])ovBS^oF#CklXfe#pq*@]HBtRYo=qgA.+$s?+0jclF8d<.g#->)]*M+]NsSpEqYGs)"
    "aG/qWDdr'p)W`kHCR>&ubq-KZ.HU1Gf3g-]Bc$n:%:;98]o1'sLBloRrDuZBDF3r[Hk.+iX*:d.vuB)TZWVHEWSe,6WiC;XPLLoVA<pMgg<C0kbTJa8Da1c5;,$uOqnV3P6hg*UxhIn]"
    "ur2V7dK_O`1h[st,L+#mDC;5Rll7(bl9*BT,DI4*qe;;hw$67kjVnb1)_PS^CmFe5)DS=GF1[2W08aHnRT>FlVGF+iD55lBk<hu*aGG_c]VVr2Ga5-PA55lBk<hu*aGG_ctFNX8a#n:4"
    "bB4OoV'%##]Kg99QaYG8[W'##"
	);
	inline constexpr auto m4a4 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#K>TX:vv8]IQWq0AvP=-,<:8Nc)n]A_^EpQ>"
    "-:jK&t?wK)W%hAe<2:]@:IBa&i(<.0V&eeR6:`&N?uWXMR]EO>=_4&D[g5lftrw<P[%_Ns#]n'uh.e2JX&o27&.)nekHq`;6Hvs84Hvs84Hvs84Hvs84Hvs84Hvs8Zj78-h@?1AdHvS3"
    "1VUv(J]fn_:XG-UJWRO]x@8HNGu&-ZJ>u%ej2*S)jh3nBSua92><CUSb?k8;TFxg.AGLn0m(4LOGd/N5U0SS>3;AtOVk]so4B.Dh(Xw-O4?fDC)6J<3VnplHc@h=>1Msg9&vh/[0e:)I"
    ",+Eh88VGU).-)t[T(<'+R7k?N:Q*v1Yw>UCos>`PPHdC2tiP'&>Dq:Lj/E6,&&X;^F[+^U)>`7B.%N_1aQ,uR%bIQYf=#jHRqZ`Bd)jBc+rU2,)3JZB&w%ZS%2o*aYK?VaabILIF6S)v"
    "IXWlsj9_05?8*&;:IAo8%&k(8>HeF:'+mb.I)c*[-^n#74l/RlO[T=oScLH+s:x*[A:v2FE%U5@qcR)/#_bWo%h&hkFWVpDngto38#V:u[pa>KD(fC,g-+^C[n/V#E32%72;a*)^Z8F="
    "ECr*qd2qOR9#v'lABuPeIZ1rnc^]CumMAxd`kpp3J-rX>s(Vv7D7Gd&Col8tNw/LG*tJ,NOO*Ad2r^Vt.DfOB(Yq#GAi_G3QfeDb$UXIfdZcA/>Zg==`Q2K6&*wnQ'v[*SmrTF@9[O<T"
    "52w[cifNi,35`bnjq;BFP`h?dE4HOcHH':59J``sg/;MlZaxk*)$7pK2[:ttI>wDK(^9Wj)SuotprI`L>t?w9x*8oum8gZ0uTi0`n_$fopt>7-&0W%S-l=U8b7nB3eedklVHPL*TnAv["
    "CvvSio%'+?p+m=I+wbR]C_jTS'D^_.s9hQ=MmmCmjT^7q_<QUZBR-N15_i#-0%=xT04=0no,sa*`;nc``U=?ksE:p3I-pigZP'D`s2S&6autiKxkHCa`wWHN1)+10<5.SuU<YeC#xoAu"
    "C/%*.-SpsSJ#E]%(l^W;,?C9go@QNImeD3S]R(v<u3Ja4fCr/Kn#L_^2i>3A')13Wp*u:=VFF[q)_unNEnsnNEnsnNEnsnNEnsnNEnsnNZ*P=:wZsWKBgWITk$$QcG%###Ub90<d$bsA"
    "P$###"
	);
	inline constexpr auto mac10 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#pk:J:vv8]IBxq$<Ew>i*>KL?Y1+Kt%6-0XQ"
    "1&,e(=AcHO^`:Q0$ma3BQCtSgr8vgivow+OENSo8slvn?C?%R:T>t/W753+c4&>En[GfoBmDSWfMS3=uL(jxjwTGe5''b6nZd3fNEnsnNEnsnNEnsnNEnsnNEnsnNZA5-ktPs49L8s#8"
    "MoSWQ#ErqIAOpGeDr2KRTWUG2(&2Xp-iu,4_IYrnA^sr=S*pV/>Mx]*ADRF`O;hLma4,VuUHw&,fk2uME[aTdvNi,&5>A=ln%RTNl8%Q.hx%kM+xfN7wUqFa5f1N72+qw.pxn7A<nND2"
    "(5e0'VuTfP86`<cQ^^JeXdZZSMvm?&e.WvFOaP[]fsP*i[.KX+a>3Il%jWVuYH>]XBTM0VY.F^8P?.@5eT;Qc4>G5`T8W&k^fqQ()Ae4K8IY^WInb+d'('nlj$,:;BZwUNjAFf[/DWba"
    "Q9b8o^=D1UXbOS^/'5jC7$XvTQrqxLh%M%`#$$*T%E&x3$+J*=#x`XpG9A-l3(tf;cEB0g9==a$Whw0gYCDsoRVx'+#^_q56/`.vI#nZ^Us3q8et(F7/?xo3bfGv#W2A:Ge^Je941v'<"
    "45r*Jc^$uHZ.WS^1M/4RG4M>g$)v%v0:M<%vC1CYH;6:4>JATl:7*5,Ii,iJqGiG.ZISO6l<p@2fH_XDK(5&U&x)arnAH*_Q+o4t'67%P`P:Wi[O'',PB`Imva&Tp,+9r'eTVas<2O%@"
    "^gOk*8m*q^-8G`H]v-ZZ:@$pkvsv9lTJ=i>QB'rFt6(&sCMK[ehG=BjEMxAY+<2G#d)]s#7$c*88GdUCwB29#pR$':]&F8sn:W83;(9--MNKJ/NWejU7YNouq`Q&U(ZhupJ5FL-5GYP["
    "c`CCJ0$_]>QCFl#6^O/:1Z]V2/E]aH;U7tN*?8`<G3uP>UTx+ge=vL(hV7L(hV7L(hV7L(hV7L(hV7L(x+41Y_JoOhT$g?Z5Q?=#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto mag7 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#s,l]:vv8]Io_9hf$T=-,=HsGmH;$uW&M3+R"
    "Vug?glsFo:6MDe>WOQn?T;;T=CN6,(C4WD-:SL_b$B#uqD=HO:EtnXt>I9>b&>@0mooTTjJ/JeAK^_AfVsuU5#phS6R)`9#####################O'EnO?<7e_^/,R:mZDkl1Otu9"
    "WkkMTV3CDZI,CFftgS+<WdCdA@tTT,oX9@@*0j><hEMS&_*Wb(=/8-g2C`a5%M*KaEu<O#e9>AEPrHljRT53=@'X(JfV$n=lU;GYJ7^tcp:*CtWrS=Us6K9PPbfJ.w)+U=bg58ZSr7ka"
    ">UnEUhQLu=h5A@XEdIG5*F[;gHq;mOER<E%ZlYD%tj13?ZJoW[8vE<>lF<:Y0]-lWo4P%(/0U?E7AFAF7U]ZYMpvJa:F7]8W?6-C&@b36GunfJG3Oi=MIUBhn^fQ=OLF>.we]8?`/+VR"
    "Yre_ue<Js2hv2*HvX[rYN<Ad/arfLP_i:J<4*/L7&9l'Sf0diZ/dM4p<e.aBmgra`-J&7$><6YJpPHLC1:<Mmc%jN>]:E&eDUrj2.A?@qdmJvsML3A2.8UHlhjwEtlA2g4OhAKa&17rQ"
    "P4&g*Ak5A@0;*DQ+a]itk?q)2mLQQnr?1(-qn=_r5HsnkO+[@p]rGMsZ$AkX=7^pbWEjb[tRa:BWYZQ3E+.]*&:(3>hnBcl4O]aH^&7c'pG+XTG4&F(0E^7O8;>3U+](o%l*HNNS.*2F"
    ".d2N;rW3UAHKjTR&AABQM'H(IW`v]ALFMQe:T9^MJw&^6=7mb_?l$MhYx)+6`I3[Euc8rG?/`h0;mT&qG&hBGB5mbVmxu0@#########################&3/DKPY^RR?7slc2####"
    "Ub90<d$bsAP$###"
	);
	inline constexpr auto mp5sd = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#9#LM:vv8]IPKCO@<w%,,%>d.2]mHEOe5e<h"
    "S`JnXi<Hb*A(.:8wYcu/<P]%0NGYBMTsAK#1;Q9&3:X'<%j?JmZb.E-F27LMjh2mpXH*-L7$QFs5kOT3:JLne><e77qL'K^[St%HZ'E,pph)t84Hvs84Hvs84Hvs84Hvs84Hvs872&&6"
    "Z?J;D(UjUqXLwH4mTfoG:b$8o;l<$Lk8tU,`'bQ'MB-YjB^J_Kcrph+m&Rg^S83@NI:XXD-Dx]b(Od]Ga_kL0hpvp;8+d@K;i9xN[#k>8WCMfYrwnA[v?B1`%3b,p6vo;t@j[YicH/lN"
    "^9]De&.TNmpB?RkUDeP[E+)#vew3;=]&qI:BK$:o1BQfBt6hZFGDL/8FxVoXfA;N=T.G:L%_q@dQkn,rT]C[d:mQ*Q(/)MluN<h4Z_%5YVIAlOa+D[`4MY:LjX*2tuwr0<iI%eh2QNw3"
    "lM8$j<GFIN`T[L1cGULOQ[OJbWFSc>tEQid/E81X+#kp)xx9w)ka:Am4<%etY,vh2'Bh96uRRQp<JY:L$uF=OFDJ;A.i?vAdF[T<%If+ceh^_@$I>Xm,sA.SET3m/UB;Ie[D<N*oF.<@"
    "id6G1]u#&@UX,u3qUkVE#(f%Vr;Q):ZE,MdZVOj>%sgq`up;;tmeul<Q67W_<F_t9'I'dS^-fqo<eBvW7c'Pn3oi<F.k(*a>pb2TFeqoW-lZi>r[*CM4*lVsQDL8ZP::.0S@EJW;YSP?"
    "'0(:iKBI9]RUDFq/MIRR*9,#Q3$D$S$Y9shX;Hw^so>tW&AX#kQ1VX*8Gwe[==bPTI'cD_e+QRfG$FIjW'AV#wbNLe>EXX/2ZE+Dh/D];<I8_6>jCZUg>]KIreSRFXLXvK(]8PQ3&`/'"
    "t7r&S)v<h8+X</8LNI>i@L82V6`5K^C#8(DJU_.`18kO*SC+6S6-44n.uN8*<d?9s27iFn65ZVNGYbi;jx/eV3'&C]=eS#($KZxsP_2^BN0Z^--J:BPE.Rt`dabG6s*P6`LE?TojkpYW"
    "dEEE5/-i;JSj)UB=kUdcR19FUcwSdaoHL2;v#<i8wo>$Duu:tamaYuu@,&RcwKEC50l1obFVdTsHNq2aR61#4r<)p1R/N]O`Hfm9X3vJ_hf:CUqc;W2_(Z77#jse%PK?=U%,[s^_]xvm"
    "3^&4Vr8gMb9@-S)r`P,QbD1S_Z8W<mTP5GpGPQMVnP3,[BbAgi_$?u)jW/Q?]hGPm8jY59#*hPEq^a6CG[JhAn#Vn/4a3MS,.2B.'lnbgq?p&AV;MctZd(7Usp-%(.kI[,Y'Nv-V5Lv-"
    "V5Lv-V5Lv-V5Lv-V5Lv-hUA,e>wfc1gWw26U$###Ub90<d$bsAP$###"
	);
	inline constexpr auto mp7 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#2QkL:vv8]IBxq$<wS=-,#ML6P$BK^d?s9WM"
    ":lY>1hvQ@Rl14pE*Zi2%<]jI48MO&MwjPdEZ&E30p*r-/X]L=R-;0tBvhetue<3x@fTMFsZWL>F@K,ko;8()Vp_wU6^ssB3b5Lv-V5Lv-V5Lv-V5Lv-V5Lv-V5Lv-3wdvLJATv:[i]GR"
    "a]>^82%c?IB5pZ$)kC^5kV7vO]bSV5HxH<QGQ1aF-O`YlxE'j@EVWcQEfST&)^LQ04t:((`lMfP8'2OdI%a?C3nsx7#LY5%8@7VXp/[Pr]v[@_rlM8Yw4e)J8Qh33_5uM9XV=Gk,@J7$"
    "i[8;1bO/*::?g$_Hb6ZE7IS0eDeHH#c8'nL32QE.fRu]GV9ZA0ffejtaC#a+DV-[buRt]-Dsb9FkQ0Dh4SPMQ5@X04)<)CCbRU<ciIQlov<f;>;Dju7oO(53%Fhj/)<L5F9iJ>JfgRa)"
    "77Am5&C&2^CTxl<rFsH]H:PCDJD)1XC#jFJEK:J4cx<)AO>`LuGd1iO<g9H(ZVsOd9$*_sHcKGQl?*d:D=S[Mi09dZDcXDB8W#i8opU)VgVM<Pu2H_/)P3BBas%%DRLn&o>E%h7rE.;."
    "x*0cH#KD(sGdf+p.h&W1PP#x7agL9[A=B=#JUfY.uShHjg]F6A'x6aZD<=&*Z6^A%>]_7D?9wbooXQD^-DFb9j&UHe8]Y=.xV%RAkE+M4rmiur/3T)J?8,jr.qdec[sm>NX1Eq:U*xh]"
    "4ve,AwvMk2w9%<SeS<;@lIQN>Jc$d&51^bRt]<u[U8E$DACu1oMR_#sSPaP_rx@C.8$b^T2W?D3tr-Pl`[BfbKUhZF)`^)#w&4LcsD`T0Km(r[9b$44A_h]CMP#$(Lf(VJDu<s&VY/k["
    "?7[V1D;DTdQ`W;89ewFZop/Lc,`YxFE$Oxpd.[aAqV(-jgUj,kYM4SYr5-XK>eo6_$pSZ4,RXj^[P,+KaGSv-V5Lv-V5Lv-V5Lv-V5Lv-V5Lv-gr]AE1d]]XUJ9k]rmh.#mb:J:T%f#["
    "HVq0#"
	);
	inline constexpr auto mp9 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#3Y*`:vv8]IBxq$<_8al*n%t5;HPgb'?7_=/"
    "I&T:<fM_%W[Iim-+<1OLxYtv7Fg@<6[j(/Y*7bT(LiZA07VYJC]JUcRq`E.=P_E]RGR_%v`-NrC6rKBgg]ijpvVB)Ixwi]E&kus84Hvs84Hvs84Hvs84Hvs84Hvs8n&QCP9_L0@Dwi^X"
    "pIU-9MS[<MhU%iums2v7vodDdGNe[c_,<MZu@M-ib+?i0VwN9Tho:gG$o/3G9UoF0'6T#HDd@xJ;*YOW,<`*OT02eZUDvfi,b'B&(VLHh1*?H1@+d69+eu:(;KgtbF[UKhdYZaaJ+mO="
    "tp.$(fY&3P'SG[h>dTT>5@*L0D6ZYP73E=.rx/3'u+&+?>>KPA'#dtq[C^^cb5qBXV]^/o^$1,VXFIV7qB*^Xl,mxU3jZVdbWu^+rs3Z_?0U;=mM2P=diQ%fvPkn[TNJ/^x(ngaE,(;7"
    "@tKs@'u'R+r-GE$D5lU#YXm#iA'SaP4%2SsEk]R<P/T_*oH#8KU)`iuHt%&kL/#A?#P,_f.w7/i?cC1:(_nT<2prWg+t`:Y4Cplu%2:)aCGGPU0IU]i[0r1).?,stJc1nj;gJnq-ioO#"
    "A(tm?ewGJYQG4AE3CGm_k'RB_;Z=$^jM#fDQbXiQV,u<2f#$h?MN8w'FMLwdYC;Ffku_oO-09PEguISsOxCn-%A19]DCYnN%01NbJ9bqoM6FO?`]u1,]4YBq/6]#,uBIKN$uA%ZvYx3C"
    "jPo0PxY1m7qlB$@)P-HAQ?,gkwVJU*-'QAR<ftK8x`)E7-`LjqnkjR^i$&@QMS%;fAcg0GFN@l.Rr?S7ZZlH$qaM_q&Q*B(o8L9Y_,8tN0pv1>Y=KoeGMo=9p#hm:-iih$qwf(M:Q41*"
    "E,lTL#U`YXGa(brYwEAG?F6SHu+CE[u)JMl`V$xDeeoTA:KgLi#xB'p[6kf4[T2@Z.[cWZH)x=-5Ynv&nvK.+FZ52@#;6>KW:v22627m>:.%-,KOE?B6hZhX#n=GYLtR[_`YV#`310rC"
    "k7aSU@[6q.?v?Zt0-M%86>m<&p@J:YXd/c4S<T:coLc%q[3W<:>mC='+&%U%iTKLgLl9iDN1aL&#dow0k,SGd;?aVTq)v+)QF0b^`eO[DDh@i-/vxlk0tq?%ABs$86Td*DZ%[sK$;icj"
    "QT-Q_Flqa',ZR]o5W7L(hV7L(hV7L(hV7L(hV7L(hV7L(H.X6Tl5-a*nFc43jQ+##]Kg99QaYG8[W'##"
	);
	inline constexpr auto negev = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-##I.K:vv8]IPKCO@<w%,,q.ox?1%KHXwZOp["
    "r_tV.a#PBobf-W.+RvnNVJ@s77xxq&&Q@gqvicj,`Y5f`GxZ*E1_nP`f6`sS/-0)pn<DYW*Jj^JGvUL+^hnDUWKHWYCEF7dTW5HfIB3LK;O2BT4L2BT4L2BT4L2BT4L2BT4L2BTVxa=o"
    "PHF:]<?sGF3+6'R0*5t68tWbGenIiq@05W;,dG@]_i;bQIlgPgVt1^Lc62UPWA%oE`8G->A_eb_V(vPPx6bu1+'#l+kLD)tB;(grEIpB.&o/72ogcTBBC8[U5eND5'oKH,Fix6]SjmGE"
    "[eQKt7MS9b8gEilO-Wdg7Hq;.lN&8tGPk^L'WmNh>1-mKSB#((gfHMIZSU/Rh/.5RJ9uTuZ$0+?CYDI+,CF3j8JD)9ee+4Qf6C<K/iQ,U?1ON_ChNgG]CelpjZu`(MI=x?+s=qeS^'eW"
    "2S))SR4Y`E6eQjhZT,4Y733U(#5HLX.rS42GE1%CfU5vTNbOFWWEq@%BN2L9hUnvnoW2Rk6EtGDO#9F9qjFp?pocE=GxldXdknXVp&:cY$kopgP%PvfR,L:]BhomfrZ:H+[@YpBoZ3$'"
    "J2vw[Y@b;[Omdnj9rfQh$Nnlk@Kd;lve@XWOn=N/qZKg;;IDvf8i:#:A8*Lld@`kq$c:CQC93+odvr11VVh#jrt'28ckWM-^gJ8%25giW&5'6;G$F17q8hm,UUSH+ZYwR=<DLDmsD,(o"
    "d8ig/X@DxeBG23v%=ZinO90cVrQ+>m?Hxb+/?Li3/U;e1>daCH63GAkg]Y;JK'XgHZC4*H^%,245&lEdt8I[f`_';]gs1U$Hg45En]7nAwm9W<Pg-1M64-1e-RsTGa2t+?Nl;*j2-<Uq"
    "HPIA^(]M[8.nt6SY[_M+x*O[c2*H&ETrm9=N4)24irfLXfcY,,hV?\?MtFkCmNw]Yb#LDssa%h[nMAlAoA0>#e5Wq;6cSU$ZJG7TZ4V4UL41>0o<6,t3&3#*j2VZl=u+:[:A>hZsYfuXl"
    "U/I1%Z9/vi$Fhrg_(S(j>NZUr6rOfXpE3s`9J3m%#R0QE3rknU(eI=0C/8K<Se:b0WDb]RrFVEJ_]N)>XP5-d6I.?['K)B,AX``d&@p0s=M<#;^)*&Ge$pc(OCT#>eKxw[mhR#>8@t*("
    "G;08>6f6J9%v.8W4L_B_=tFfrw#u<a&%J9N-*S[8d]QA1F1w'`,AF#FOkc4K;tFV`@Tx@gJ.VtOxUjmkS2sSJ6b`4:POa+2v3anc.eLI3EjaI3EjaI3EjaI3EjaI3EjaI3r$s>=UcD`>"
    "=-kdL5O81#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto nova = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#xJC^:vv8]I<GGb:uN;-,q8*w7A9?TOaLrj`"
    "xb7j`7)sk'[+qVI$O)+.MY/-W=C:drkhh==6huY%k$sT-B(S>9/Ghc&QoULn+ZNOXu9GE/<@gEM<J*Xoa].V39l0N:asA$RGb*d.mJ2N:asA$RxiD2>3?N0i'C_tBAKWNe*3uwBc:iA_"
    "_F9^*2dI$>mg]0n-<+B83G(4snx[(nSaSg[GgQbD2Q5rJF[J+Sq4x[1$eeQ-SoF9ZS07TTQ]p*PE76Mg>vSD,#tdJ[4&5g#+n&<12'i3j5Z3*AC*iWLFZWNPJWK?pQ*Y`XsF]WtbW/u1"
    "i^cCX._-iEA9M7#>dsSO?sBDcXf&lha#.?i2;Tph20rgT#.78sVpH@//?uE%FT+RC3uj9SnA8ii7jHtW`*x/m29e3`)$hVUNWn+[#B^pAf#4,%axjk+pplAPr]_%i>LKIe4tVX(b3F(X"
    "QFXH'.QM=]?xdN;^4i?;:H`*I-a[NdVGg:GKwYRfc$,C_4m.oP[5mt/9>#G%:4]N43?06YP?b?2H5%lNA-R+cj67Er+LF`ii-IiWIFMk'%*CoO&Ah7%Tb,25D>5bo;,e0HA([s57&7kk"
    "X?Ed'kH(x]r$0i649<RSDwkea2q;n8]Px/&qbTNPc`f_8/D>t'QqNjXm?P:f>=+#VsW[]VDZL0S+WM$Z?Cx_)[@3soR3HvEY#9CL$Nk/Jp]%I$`N8r8/2K,F[&#Q7_pK9rOJ4]U7_Ykb"
    "O0W=o;cFr2sTRCXLiir]L*Wi[5+Jk5VH/F^F7#_I.Q:Bh/*X';Q9H%sVYc:2kW._dQdjuBt4)[DG&H6d,*[k'v)t5;+h`gq^s`fiNb*d.mJ2N:asA$RGb*d.mJ2N:Xr=DC--igALX%sK"
    "Lg*##]Kg99QaYG8[W'##"
	);
	inline constexpr auto p2000 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#r&c]:vv8]IZToB`^S%P):BB/%fKh+0jCcUD"
    "7@])at27?B^<hrEo-ZUb/A3.8UAXli)Lc+I;]afM4FD->'I0%BSG7a384T:6>Cga?u7=k0<IG*q+dN06a/EPs1B'<`HC%######################p(%[]CQ,aLCBr78tvhJ7M$'>/"
    "xYB;F`jVBPx<mYjgl8380#wt0=C3AN;NB,F.&+;hexA2QsxYY)<f<r>3(1W1N0WNAm2k*Zqua>]/(cnM4W'%'rhgLA:Fo6JCHsfeDVt9ah8>5k5pW.;bQ@-E<;AK,UgI/(GB-9rItb:W"
    "t41vq/Ye<.AV$%'O7@ORXFb.L&Zt[A^lZ6@K7viN)nkG>=?\?>@g1$4?;w=4S)21f1L6vn+CwVG9#N=-U;VAGh$6NM*;O5S2;1A#07$^ZZ.ck86tFh/9;[7vd*OvvpbM@M6#C>&f4:Tuo"
    "LW,1t88J@/rBm.el;D[fap3SC%sM&f1-7pXg/F<jJuDQ728s#ZR)gTbo2=wCbm,nDqb`B,?MNMWp<H6<Q>:W<rF&nTI`VSJPqCM8<e'dEHb;]gSE_0Ss-J<=gY@mno%im<D5Qa7gU0Td"
    "hrgJ9kx:ad[l9+J91?cc_9hj4u5Xd^cdG$[b6>^uojjomR-NJX9o8$0D4Dj@I+dYfWd%]NUHDG1QOPCmC<'ZE9fcaaQxPlU5CrggN$t*3TU`WiQIY9GnYj6@:Zj:##e6vTwA#De#VM9'"
    "CXpP5t_F4JdB'=Ohj1OAOGmMSYopAj)j4k4WM'gm$/h7@i?2NgZGk%4(MYPF:[[p^YF24[(vgC_%LL>cP.4;4Hck.=g-x,;3###################bUwRnGQ.^*#=a.V+dt.0#####"
    "D2&u8LsapL"
	);
	inline constexpr auto p250 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#f3J[:vv8]I^mX$aQmUg*xh^VAj_/W.jjUhl"
    "8@[q7Oea>?YR8H]&O7E8^)rA(#qRbgki$g[DNJ>.ufv-.T+LFZq5xqMPqmTakYosQ#S/HNV<m*q3kLsCRe)?CY_ZOsMK#######################(q3UhVMRepevrRJ&2uvEp`uYJ"
    ")HEUFw>Q*UA&WArhGQ1]vH6%@gP53$rxJ'e%U@V^sfPqg?[lC`UP`3n_v0d$'[?/W5ntc>(sJs=]=G4<D#;1Ocq&;^/6<5`SGQw4?2E.g:]q^taFJY/jFb:<dvVZ]&[W&lW-B)H7DSHs"
    "k0J4]t/gs0s:)t+&UQ>4iYYxhO]lS@]WhN:BII%nm/X8=?)=pXHdO$=dB_X=5P6XRM>YH*lA,;Pd#M8:KpUulZF6e;+M?FXAT>Itx_t<i[47-oaKAkGES`hmlrb][9SE3cJVo.jwrlGL"
    ";C1FiFSL%ZD[%E#*]T9>`k&D/QrgS5vWZdts9RU;RWgm:OK,-J@bQtdC6IE'%4E';'vdGk4;dOnclh[O*Oxa$iN,T5hA4B:9Qn6arWlpA`uBj4w*ks.m#ovua7$1FVw_/_$]F7]xnT:F"
    ")2e3N*n(^g-Koh?/JpMbCwQ1J(su`d0rGV>cJPEn?3QaTA)/DHU/1tr@`0M=F`9?r;v&n#F;8)C>ibNU).&5`dPcpAU#].Xad_81'%e.meEM`qYuw#hmFOVGMvl:i9,r^TkXA5vK/@J8"
    "T[OZWP+F*e`c#Q1JKf<Rap_kKFF*lsaTbWCnAD^?UxUu5wX@4g7,w-`H%JO.R(dt6fpJYMtE%qX%h'f(h###################MKV.h/_Dw+NZdA'FBD>4#####D2&u8LsapL"
	);
	inline constexpr auto p90 = decodeBase85(
	"=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#,/@_:vv8]IR^$1Ar,&k*k#+`Oe71WQifJm$"
    "v.&k-Am?^Q%UkS,ljd2SYIjB(%O@4Eve?1OOCIe@r17u6.'x/.C2S>:,a6CeN*INkTkv&n15riewnCW3WCcZa[nqbU<)IGU.4/1T8U7L(hV7L(hV7L(hV7L(hV7L(hV7L(xRr,Bu[_Io"
    "Z'k$<nKf(94%c>HF/2LPN[kq`Tbl41gi;F+9P$@US[dZ)(eTGYP3c6^B8t=/(eR%8C[7(,Nn1HB'+(S)nDl(e/$)sKu'vM_]CZr,d4;v`bUTQeLk>D-@OjjIowE#63T)Q%NTxCB0pn#C"
    "n6,Td1O^eVrpd`7keRO1G=)[/5Q3FqA.(nScs.668``N[DtZ-#_nmjP,Ce3F05e8H/T_qVJ.R72qp%1Vwr+<]bm)NP3b+D,SX<(UVIIJgq]p6R:?TsLP;]<eN(gEZ(Li$N5]=^M.6UXZ"
    "kEX]j99KA0M@?*)XSr;nrDbBA:im'[i@LZGp8.,uv?TJ4$BUjEibs.UAYF^UJvg?LDtDGCakkM.;b^M$r)[Yk)0CJ_0ipBQ&(&`IqncA1M_:Gd3L?nh_T%vX.`j0CmogE5p^I8rXG,uT"
    "eC];RL'vM_C6'Id>=B%c>?7uX(qVUQQ6Y$Ir3jL*Q'[koPncEg25T4@ZtrMY?m9Bg&$STcv5?$Q`>bfZxH0SIAQ]]hurr](]9hf:HYb+'fkr[eY^TPgf@l/CN.Ox[;E,gEI-Aq4[onDK"
    "=%5C%1:@VmJsYS'YIbA=iw<4]Q;iuV0JJJ2tUa.Lcs-xY'r5n7F]Zdhhk%@2_mDIP?Ji7_.(e$XZZhI]b+m`ttF_ks(P,n3Qc@L%)*iWBqaMYCaqi*i6)VhpN?M/ERP'[,q@F^c&Aak%"
    "a+-d/jHS`QK(P<0CMQ$XaFrQOl7IFCeTpS#VZOldZ0eu4lFihab)qp%m6:5%a$iJl[/l%6F/Z%D[gZIt$x4gqa]fqGu][SgHb#D'aQrmDJIuq<i.F5(Y0W>s0eGH+*/@EAq%r+2?U/Kn"
    "awmiNZB:XFb7>cT3Ql;JJ5p>8X?rY2D/$Ubbio;<mOvRi$042nHtBM_%G;lpW,^f7>.l++)JMldNMD#V`Oi]+`19?Uft@:Wdqil)Z,1EYK^<C*8eHhb*Kl2$t9')o>/cZq?8tJ__[wId"
    "nPCS<3qsnNEnsnNEnsnNEnsnNEnsnNEnsnN2=OQ(>W3Sn)skqBb)&(#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto revolver = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#U$TP:vv8]I/$)76_x<i*(lCuN#c9:($@ltM"
    "TVg,=n^kf.M@p'09BAqMKbK(.]4$O(k[X$>B?3iW1M)xJ0Sag:t@IF/oa`w,mP$IZ:R49)1>eI'S9g)fh@qFa1Or<O1E1sC^BQJ3EjaI3EjaI3EjaI3EjaI3EjaI3qDQU]XM/,Eqf8V_"
    "OR%Jm4,)KuZ'v#MYr=DR%f@giEBCg$&;>O-77[@5fQ9)/<dRC7+KIW:SJsZEox3IU1N*<.rHNJsqe[Bd3e8TVFgv)=c<@XHpM,RS)FxHa1.M'9;sh7+uO]ZJI/:]W`x6C;W@ETr[+17P"
    "RoN4-6s#0RrG>deXm9hYq#JFtJEI2*?8+G,U;Ktb?)S%dCjUBN^26=^Dmtl/]8IPR9bej,')d6>_v2s3*:D/.>)88j//N=8IB2jPO+g1S[;'aq`kbQd2@X_^X9.?nRF6ailIf%Fa/U8l"
    "M)Ph3-fG]Ei>t&Q;'L)8lZp;+bWDkS?NpLC5Z1PGka)M^WE<8&)vqggMxM)'?@$BpkOOm)(ce.U6es;)+i0t&m4FQ61[B[q;Cr/*Umd5p4J;VfXm]:Nr%wdB^gZ-_=a7jWWSMd@#m:KA"
    "xML),l'rb-Vp:]?L:2'VLX%HtHB*QgdBIv?^S6$)Ce-hn3[H4A<WLJ>Dk3WVkVD`;q`C%=wMx)[u?ME``)ih+^^'GYc-nk?'qAG6_b#b+nC-3ue6sRG=%'KbUiGGASWIo`gf<?=&[^4v"
    "@9[@d']PCUVVG;lhiYMrKe]c+1R,?U^X*Oaq.8@SVn]#E$L>i]ZdgvEH8?]YT1[2qI/mlg#e6IV](Ev<aR@;E8oou&SBHbR<0o;0/q7S^.8Xu_JHXBl*5:snam:K=2KPx1^7@+5wXVZh"
    "Dv4<Ij*Jc1+;+W<i;JoXnZPgR:^[Bs5dFS@`bQ)r0MHS/'-_#4YO%9VZF4A1Hd_fpVL?qgN0(?q8A<KU`ib+RFGYW)wu5Cd,J5^TL@Nd[6(%)v'1G,OJW:Ze-b$/BhZIqChZIqChZIqC"
    "hZIqChZIqChZIqCYGhcHaUpNab*9v#lE+##]Kg99QaYG8[W'##"
	);
	inline constexpr auto sawedoff = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#/C_U:vv8]IB(@[<ah?-,;F5+sXca9@lJ$`."
    "Cx&mbMWjhjY4>/fK@.$?kj9]Z@m80[L`X`7P.[bGa$aW@g)wT.w`LWuxLoDp/h.0vXo$]<,vRJK78]Zq?ZKa`I%B$RGb*d.mJ2N:asA$RGb*d.X@h)4pTbQ,4PDhh,3<4M4nA'7xhC.1"
    "iV+=ov&wNq&ExmXD#F2rW$:jl69mL9*Ht=@wX9rnjH6p9`WtW:keU;LVTav*Q`5W&56CoC=I06P+c42n'I9T/3H?nKG4RqmeP^lKD=I+T<^3+&H5mpAv$&6g<'wNq:`FfVRWOn*YL:LO"
    "(Ae+*D$uF=Z@kdNqnlWA6g3o;Fkp[<[W=5km7(1<F;pI<k?'E#Wg[C4sjv>TZr#k2AnjFuWwR8LXk&GeGn8x;$3mgThk4o#$%M$CO`]`sFN$WDu>`$Ysc#/c;.'%:3O-JNW4(Xc;+w$_"
    ":,qAPOHc2Q5xvES$2/ZX]]>N]j<Y+(iuW>^0WDbBa75gk)/qa<^[h?q3?%HpJ=cqoK.ra]j<ba5)s(J7t);G6t+RAfJ$S:q9Jk8LH^;]6<?Ti0-Z%Xn&gt]g$wB-gc_EUE@jJ3=/@;NC"
    "R:l2Y+S,K]O`gf*#)?=Vs>>[g,bhB=VYEHlS6x^0,<d&h@>/YA+=VPpmhiK0)doBOQ1Na:4QY3q8U]VZMhnLm_A;LNF'^&1-T*>p5AUnSYsa:2nfV.^rwe+WTawJI?[_5t/F5=XTBk5m"
    "+LUh,u^;wQGb*d.mJ2N:asA$RGb*d.mJ2N:nriGe&C$Y6RIF,)31(##]Kg99QaYG8[W'##"
	);
	inline constexpr auto scar20 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#kKf[:vv8]IftQH@'0j+,9WB(XMg4-=BSvXS"
    "]n@H+Ord.7`$#H%1G&RruuJ^MPx:2W.m/<l:i.vOAUJQQK$t,jgW.3)Y*[dOuw9l^HNWfcR0tAT,,RIUf])2DvRH]sSvB*Ll?st5%7UhJRDE'Vi<hu*aGG_c]VVr2Ga5-PA55lBk<hu*"
    "aGG_c]VVr2=xK9EFbX#'O/ll9LmueY>5%U8%S%`e#Q)ela*s_?bf=%qC6-LfX<AD*](UiumwfbK'c4bbI4YuP)m$=G*k0ut+:<pL=Tj$0mc^&DL;lAZ+mxbhf(R;b%(w_i)%/4Iu&2Yu"
    "Pgwxm_f,_u+wom1sn#ZUCFu4#orNhI7SQV0Xp@%=[PW_$kDpGD8Z3:#8)[`+Mlpx$ow#_jCeIfUO*)h0d.8UbK%+/udie`c+$67o/1ek-NX1Ci$MC*5UpB'p_4Zb2d*#%?#@E2Vhb`10"
    "r609vhT'P=ntL6X/AH:%V))XdT..kOq83<9c:gUPgA[%2^jCsTf*+K0qq[$nShpuB`1SM=;blZ<jJr1U.<#;#:L]/W@;ocj8W_r*fv3@MrLAV7[;3AB@?=VeADI(,j#3xTAACNR`_GYb"
    "2hk*2n>hk8UbD(ZtIoc@[M@X]Y=Wv9$-Z3s19Xg5+v9%KB'JfI<R(ABfQ?wK(r<EU=4RWaHQ1Mo`EJ7uZS22&/cq`)LrVT+VE4i,X1o/,LLW&YDdK@;s`(s9R7SZHMPAfB6(^65#GK[R"
    "4[))nt#Q^FB7vssB+&#NR=xjh]Jl&*=vvHm^).*WXYGtJIE[+iJ/<Y*f;5;rCi;?htei?P^b<]28>C:Lix41@KYsKtsP[YT;*TL3%_,n*k'p8_9jS@)X(@iC]6q;-5cjgTK5Wj7r$9Z9"
    "/$AekF^SvF'6?2g.-9t6A7YNmb1u+%;K`7A^N2BE2#V_uU=vr?6q:@VMqYB60-3HD=qICXxiuq=ghmBAIJtv@IbC?h-*AEhTqO@eo7bjc*3x`fX,5w*.8x%Vx-5fA.x;r<3CWXYZ9+Tc"
    "]<3+Ni>in4Y,UR@r$NAJmRPBZIC9d2a];/q'j_H#xM4.ph2.82SuL.$=4S'i'GpK.R`oTn86u[p`*YY<4ccTaehedhhD)C]&Ioi<2YC,TY)+@igclsEij1dr?K^[U:Kj4c1FRE[QAG`D"
    "+@KJisBD8&C]ds*6)0[DL*?%@tb'/iW>GM#ot.EU6,a/0^s_l61E&q<2rdGj3aud*9'T[N_354t9;fU1>r/kJEiUMtD>fNME1Zi<lxh_3ad?G9/H6.m>(v'Mmt<C>N+DIRKa%%X1U0cJ"
    "ekJ[1F`HR6Ub[J>__3*)w*QDitiY,[2%5:YmJbe[AH6V7Wd.HJQ[ur77M&rIrl9Y57'7LWg2j:Gt*B4-uxwq,d0<,6Y1tLp[[Ch388uF85-&+9j$'7YH)-=XV3Mvn(n$^8ra%Yd5mW.N"
    "$m?wh'HG_c]VVr2Ga5-PA55lBk<hu*aGG_c]VVr2Ga5-Pa%rQYq:[G;G)?0%(Cd<#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto sg556 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#%U@K:vv8]IR^$1Abk`-,OBam)_<hHYNX0YD"
    "-shwLeH_?To'v-AUE'IIRc)O=K2xU*g/<TPOj<r*99*QLY5?^?BDpDc?=^NdL6YUckHt4^+2IV6Yq1&6HTEDnW(;arWH#Gn?IvZUS>uf]e*uU/[5Lv-V5Lv-V5Lv-V5Lv-V5Lv-V5Lv-"
    "o+Oa^N`XZ::kCTgEHUO/SGl6s[vUmZ#^O_s6F'oqfI]u&/5bLPwr;?:7[hdQwV`#DFdKbTT<(gmRlLxJ'?%uW(3SH666q1(dn1@Y#mF6)fR(05WJtpe@p%C]2BEUoH<xTkWU)7A<CAJ+"
    "wjZW(uE,L,3.w[h-:,8/FT-Ddqw`cRHxw'Nj*5u4jBt0<]5%*fm<PfT?w8(%DC,Ae0EN#tW5eUm1UnfY#v1*+XTnRQ=MBCXD/'a*WbGR*'@rPau)XQ5]?>)+Pw.*dpO7$^<)3rimgU`Z"
    "Jq,=Pk.?h'#dlV=N(=iFg(`UCNZ3X'0k8kF_mi3$#sPKrrV6e+X&@]_xu1===qsRA3bb:9P6j3$i)&eoC-YIi&p)'[/)>0Z,#%@t3u+<V@vqom-Yi,u:j8>)Nv<v0]gMvh?HmV;81>T?"
    "uU]#$0Psig$-h(9?=te'7w+2kKr:#DB;WZS)M)bZ;=VO/Z)WW@wPrlN%c;*O&@VSrB[@?qqCu0R]dWn&WVgSf+)(toY8X[ER6qrM']#`ci$*g;L?d941i*;2H5)r`B^7_*]3<&:8MOUp"
    "WoPnEw;AT?Iie*/#?Ln4ak$pckYk<;/%'XRtTWxQ8psr^NF5P?n@i^t7YGdtCO]cc5w[hb)`'g1cqSuM*'&GfM%6&dmD*r-X0=Qmc0k[BhAQO`9C`Dt^I9&p[OkAit]FL]E(E(nqGO_U"
    "G,2;LtF*6lLoKa-4fZX>=1#RBM_hqE-rd&C4:OmUjKuw5e7qdh5U%sD;1*;,e.o)Yp<SfhcuXZ$;7CK5b`]s[NsSBtMnGuPB<DUim`=DZ;^sJcMEn<;>AwY,BvaH'<fQA_oAkcZowAw`"
    "#'wjhk,G(),;TI4Xspv>fOH=WVffb(%)&(;fod'p,lp2VwNaS`6_O'fZon)t;@ZO3iM(ehT&1#'FN#pm#T`-,Jp@O:Wt>s1?b0RRb6*%D`g9W=IRKx`12p%gp_N+BfK]=d/ORQ[+x9%."
    "hgQ`;fXSfhIVn^Yp%O3[4IHN:oGQ/X+OCM@&Ceo52G.r]7@)v8x7#C@v`_(3u,5ihqxZlo1R@Za`GT*O[BXNU$pw-p(hRZ&@B<=Qcw9oNEnsnNEnsnNEnsnNEnsnNEnsnNe1s0uP6T+v"
    "SDDNi#i$0Yb%###Ub90<d$bsAP$###"
	);
	inline constexpr auto ssg08 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=NSi.LNSi.L15###1TX8BD%###1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#/<FL:vv8]IBxq$<L,c-,r1f]?NA;Z(DsDv("
    "bS%wfo1wX$+#%P)dOjd2b*s[(C8=:grfmJ1U*p,j3:@x;7dSpT:LntC6mHhMhLgk4ttZ3vL?.1H$PmW3)/W07+7F:$iLarU>r4T5vnHBsm;:$RGb*d.mJ2N:asA$RGb*d.mJ2N:asA$R"
    "Gb*d.mJ2N:asA$R@2`sR[[#6IKR:6Ys#kFku&]hWw^wxHU@:>042[<#=ok2>d.d$f@->cPmJ8M[S7)JsB%;wfIp[vuE6Xu]I#_O3L?2)RTuE@OJ9kB0()V;Y>apU0-Eg:aoGvQCeBCp6"
    "t<J;DtGHgdo%Ex?RJ50#WN8MOKBTBWTVs/cm`/%EpYj4a5+i/Dj:EF+)c4wKb_bVm#=.VSrH_%Qnrm$j?'RrZpiK::s<HGjmqNW,o3[w=d<O]`C^clja+KWmis^^d&g6p^h;_pTF@_m."
    ".Xu`%JC_wpJ5p:*vMw6CrHX*,GA/%qjsDP@N7DK5e3hd+,&QI8HL^JKD:Taloqvii,[5i;];jl%F$JWW=*M>*?KWTPeMt1[$-<-/fO[p5A_%l&-c5fQ=34C,,u[_opZ?D5U8,H*Rn;t]"
    "duC6PtA4tHKx_e57h5;LAgDfiJJKq3#Sm-LUc].e#T,U,fH[T#wqwC<pJ2<n0crTCD`lsf&NZ1RHkrEA=f0+m1OIA^^_;t>3$FhSh-YJ&Mn#%?lNZEZ/^OCaaW;C6BEAU/Xc1Hpnl0$q"
    "WZF(t*hHH;ilThX;PQ;`&_#A]851rgJ.UMNv`Whh6tW>Br*<;L6.moqh6nXnp-xro4gY4<fqH#qC4O'[c45I=vx^H5A^WJ?wlh=JpcAekgTZmKWougj@Tn]dlCQcXB&%k;b9fwM`g866"
    "c`HW?O:3An98Ow$]bR%`=dR;AeWXx[[]:(Zb6=;QrqH;@AdVPjJ7`$K&*SORKYukh#>1qHCtPLZ$#=mBr*<;LJ$C3/w28iH5*VeJYFa`g4T=$A:BRk_oUIdAN1:B&-o?FW[>]CuCtM+_"
    "rOBrrK4AuR^@)lGY8w-S:8(<3w8`747%9^URRWQ@wvE&qg.44)ns5saI2qHF34dVW+b^cW6CdmXP(JTQl22`UDV>UsLV7;VGdw&*e(4(3kYQ9<`(Z$Qh+>4])x>9k?Lb%I0f+TnWTIfA"
    "IUEg2P3hbXp9'Hq-1E[D[gXO:j.W=lZ@uj-8A&e.MfwrE*@fZE?9O;6]Xa;XqD<xg4S/;=WAB(qwJU]`?2reNkIC'-GT(Wu4Mvj<pJ2N:asA$RGb*d.mJ2N:asA$RGb*d.mJ2N:asA$R"
    "Gb*d.AKW)4N>;;R*oZ8B.3l&#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto tec9 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#8vKM:vv8]I0*276R;n),)QiP(SGGv6SWOJ("
    "-d0K-of>s<USXnNN1.,W+W*E44'ZwNij>E80KCv/YRW,1eHsMP;2X(:%jEj(FWkj.29a%vV=a>k0<?[?_3d--n$,gp;U2?`CqP&94Hvs84Hvs84Hvs84Hvs84Hvs8?WTgWn%AocH0aOg"
    "Z0&^]BIm]Tv@U>if>?.Ji@R]/H?`9Ua`vh/qY[^Y[ZRRqfQpvZae[EQeaC;^MJvR1(7[68.%gI:U$&a+[I1PaoM9@,G`=]d/SFboN%$7OC_4q$XN1<Nic-o7o$1#[u8N<E_Yi(#4T3>c"
    "_EuA6dm5(#6;0R=^h12%A40^0Sn-siV2)>di>)g%2q2g9Wx6c6lCZw8gA6*%.9.+;eU^jgF=Fx&k6<(@9xp4;6<4.:.lR,sHhN59D;cQ4<;ta`QsUE,tWscEC:MnhR2@q>X2K1LVf$95"
    "#&2-,KFNil&s*p3m)ngpHDqtIhub-9V=KI>l3R)t/_L+;-ks(8G0,ssM:wf-Jrg2J_J,q-5KQ$7+(&'dJ>[vW(?ugn=4k]KZqtfdA[TxsK%2smQqU%80B.ddo+$F]:+I)tHT,4^s?O#6"
    "^G0xe22`V8]Wi7#LYe-fYe:&<le`M&dcus<[`J[a9@,7dRN@RRNO(d,XV?n#CP`Z9J674h8J69(#A:VBZ1KI`CIJ;c&i2+G+@l1Zor<.j,_r(gY>2aO%&.U0wHwXQ-*553lrA>7hM5#O"
    "DVPm`-odD7kT31^3d4SZGsX$htZ0Ui]ECld2_KfnV4c[YiZ@Yr9BSkIEMx)43)Yq8FH.3OmK4gH&r,A,*XP^hbo('fltv7b29Nom-p@'T',vNe/n_jooL9=*Iv=S[q+@VbJ%nj0BDI-^"
    "'^Dl>]SgJ8][)vmg:LK#OKjhi1a7aS_F0r[;(fC7rYDMO3VkYjW3BO^uB41_?Shp*sGgTT4(Jp1prG>cQC_dFj7Zi<hV7L(hV7L(hV7L(hV7L(hV7L(=2(XQB@+^K&f&:C]/khaU$###"
    "Ub90<d$bsAP$###"
	);
	inline constexpr auto ump45 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#X0^P:vv8]ItG$tDg1i-#qrLCU6b2`KHjwM;"
    "Pib'F5W&paMEr>*&qQ^<MuX.=Y8p8'_C'O)]M4<Sm1^Q:^.MpVp?PWOxHuNT(](%/?F@gJKBHY3EiAJs^?aNG0pplsIRPNuAniw;Ga5-PA55lBk<hu*aGG_c]VVr2Ga5-PA55lBGXu[r"
    "_0AA,Lkd0UKEXs$iUB7UXI)$O*w35l9Dh+6+lpx-d+6RlAtb4@U*Z.-lITEK4:^l-I2SkX$'ki.;3A5?7E*%Tb7NC4-5T]($D<rDCr&9sQ)ILlaZ#,6e*+4dR+x^&tckkmIBK3cfJt.>"
    "),sbA5uue200Coc#%W]t`nOQ21He+HN:7,1<(3,uXE(elg()$r;a[9VFv+bYjv=uF_5rE>F71xbX)S(A$bh^_$OJ>1;^+7#5rsrLP7%^tb3Kx.eW&v,5Ah;%hq3nb$,cI3(^v%K&6Zl7"
    "p5Jkl]&RJ'Z%kj(N3Zv[_[)oI9PQp.I+e:k.q:rZ#rm+(IdIe`)LeD*xT?iQBw<QA#hrFOYU-R@lO6+GNd[h`H%<4+rhe;JJ8jr+ARo8c^Lk<Mv?xcjYOqb)V4K;:0Q7;#^h@jSR/h_M"
    "C:SaVWIWXULn2=+j0uQf6,$qS4^0R:_&YH-b]u$ctIY-XD))`fP8^iEC@Du11X,QD&mD:f8XBgX6qJ97;b+`T_STqHTpDipx/MfI'[ZuK@l74Ye;a9:ulmwO_MlXq,t0e$Y=[A>X]n[f"
    "-26#f)U$si^/BMMcZtnO]+A#%^Z>0U%w1NZ);@$j4Hj:F)kMxebbgvC>r-YW]Gj+)]a99,`:)DbE.HcpK#Ctd;WSCS]ma0tP=]fIm8_`ZRb&c9+^YE]DhaC)NRPU(=k1nunV@ndQ*q`J"
    "n@xklnQ0<S,0(:o-cL,=&0PT2o*(iQN>U%1hj9686fKEUlg4^;cdV[I+(]wBua+lrt_Rs==IU;O/Vv@GpS9A/wA'uhrHYZAp(RK,4UkqcThS$e;W^>QP&,$'th^v&eI<B%nv.Klc>9:L"
    "s`3,SOtOtaBSFkIYI<x2iFo_e0g>DZSo^g&$CWj;b@m3L2WFTJt'-V*w+(Qf'](SKNomMlioOd^amoxF_v=h=e9aNt:]B]Jxmr[_NTdqVG[(Wq1/Sn_j(#%VYc%gEOdD2G(KW5fuZ17*"
    "HX/xl6b&(#c]+Sc$ixq49$j+GwFd/d*.&k^o[*0>?D#?jvCOgdh4muB,UZm^Eg1BWXDsV]<B-l;el9n,R*wf+1sf.lP%=nJtqH>jnbFG'=_EOHehFKmF='`D)i)vZq,Pj<D'faA;rCXE"
    "e]o`crd-lox(OAmN2OGr`&p?WKc>1HguiD`;>qn+^-rT&QGHeLq56ktcO(Lax?SGqK@Mp[Q/aZA3f+ORb[?_O(-m*DYH8/hx9U=9>2k=Fb75@NwAo3N2P5xF<'ZCkhd9Y6_Z>:SKSjZr"
    "cF3s5@$HP%s7=/FUp%B9La6+b:Vc_8:0A%(V7`e;0ZXF/7/qmSR%v&s8^(]>C7F/%fK(]K0w%E5$;V[n.&#@'p<`T@(t%VLHxiSGLfQRlFIPp;U(^5voJnx3DEGNb61*`Yudw>=;lm#?"
    "QS5MFS%)f`>[8j'+>Ya`m>s5vcYO57K8VgP?rVqL1k4&]HWsQY_1EE);bXDKGa5-PA55lBk<hu*aGG_c]VVr2Ga5-PA55lB0*t%EsJaYHP%x#_GUS)#mb:J:T%f#[HVq0#"
	);
	inline constexpr auto usp_silencer = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#_`fd:vv8]I0*276ZS%P))w2:cBf0*cJE7:*"
    "i20<35srN)YO;gM9`XaQ4DJq7Ik:j>S&YC4-cIZ7KWgx,(-aD+cxj$Bp>0M%AovXtf[BL:G+;b/k'39vb/DVJ1/-%H.LO/6L########################vK5?[?8GV0S*,l<>K]YO"
    "qYchc+^Sb=Wal(K2h93=5PlS$+a$n]s0k3Qap@mfS5[:TZ)G]K,BNHas?&(14RD;USC/bVeLuG5N[st1Y6Kb^oTN_sla*S,C,fFs5?cB)CXj7@KJEJD@;GYX-rG4u+[1m$OE4np',ngT"
    "=[h;[nYU*1a)Ub'EQj>47I^pAEng+prin'))Pj_6E<Q7^l7^oj`;V30S>L_+&w`<ioRo)$gdb/ZPXx<0DMCn9cRX@k`gFdoU?GVo*GpM7N?98?BHPNX+$$h`5)$EkIW)g#1T&<>X^gC3"
    "hG7Jav7_IR7FdO,K.OtF9Wh^.glLU4w#qvV6LtWY@H&<9A6_sVbg=&1Iia83)_(E9R1>Q_Gi3B2;6vj/.*M2`QTYt<j^3LHCQ+,KKlE`DF#1Vhwx>?,5_>k<;p4kDw)HHEtAU(W&j(V^"
    "PSXjVN>H/+p7A)n$V&cuIueK^X]N83SG(K`<Q)F3@t]Vj4KwIi'*/,>IrZ$F4@A5<<WiBU2bvY&]5g86iHxf<Jh<%i5JQs)T_Ko>bcINf]?TS=Ld`oIO(W%-ul)s.rkXjtGd$jWD(XL/"
    "g(p;#_3LCUd&OF8s@1['Yidm[sdHKFttW$^q)VFMUM4Yb5P$D&K[c$i(>Q$Nt*t0=^wN;HmP#sPuHS6@3E6?XuZw/5@'eZ<]vHda@W`4i2P;J#?p>g/QB[]Mb3=m'78&wW>I.W<O8lJY"
    "lJrjpBsd=X[K8t_TJ<pVHasOPNQ_^H(WXWS>########################e1gK)mOQ#oVjLL=/.%##]Kg99QaYG8[W'##"
	);
	inline constexpr auto xm1014 = decodeBase85(
    "=pxq9_8v7&0C/2'K(HH=.2WlA.2WlA15###lB9HI[####1EHr9809Gf)9+##%>a$8]=%##mTU#u43###:nq::E*'##Jq)##i)ZucJ&c-#[MJd:vv8]I>Y(C;bk?-,6v;GQX`D/iC/#`-"
    "BRw:X6*gNWi0$C]QZV1cmZ.Z?(mZN3KYrMQ$r^j(mZ8MSJ3sN_`_#`a)Q5.5CFLwSVx#@V@<aop)2`W54XidCpUm3:asA$RGb*d.mJ2N:asA$Rs<qoW0Q8q9IuJK8k?ZcDfgRB@V&:-s"
    "/g%Yc1^U]aSLEXFd]bN0M8lfDN*^3RPRcHRiWN@5V@KDJ<4R[dJcf_fBG9q`e5Zl:I:Vh.x$m+<OHKm875b1R03u0E#bdF)a7dfk5BH/D<Vuj]0nSLk;S`mWYXY[+-04(+1WP1<MSjC%"
    "L#MFje-TMGXT(j'jRS99QgCO43^88^l^n*WW.NA-sQP;`Msx=kIPN/m=1fgRA&0Ud`A&V,27FmqHomJSE9lxrs;U6+9+J$Ws?Gdd*aL3i:=$p$)f]asU<)9;`V@Q<glJndu)+6kVG-sM"
    "7ec?qrj9TAhHEb'8mnC[n#ci-suxv`Hh]4j]?Ri5#q(>_pt)Nq_D$scN1%bI%CARip+-`^%/N.jA5H+HPRKT-DkLrhU[;VG)Rla>^AmKkEqr*VP29rrDYS::Iqt[*a8e)icv:4Rc5j.,"
    "eB_wU;_Nt_*17/ZV6KtEZ_vCEZEhZEFP@l;_sPBr1<_;[Zlt@2vSA4bO(VQ,fXBF:KRV)RX%w0ns5mFn:Ij(2,o4%c29?'?v.ajdD=4EgBkZH(Zd`9Mr6>[ea.(M=@'lfgZfF]=g&o%c"
    "H74lDJx1P6'bx@>kvNqgO1kaHtj^lY.KA;tfq6J0d=C_$Vg,+Sq5_aT_so&K'<$#;r[%?c+ddWQ'LYx1E=AT0no@4Y268.G5/p4F9_VoB<T<QX9CKjq^&W$*.LpP+@F;%.>=>8YG/F+X"
    "p,e4->k2uo($l9[]mfA9<agajdpm[/&tA$RGb*d.mJ2N:asA$RGb*d.wRhX)kbYo[=0nM/(8>&#mb:J:T%f#[HVq0#"
	);
};