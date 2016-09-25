# FaceHacker
#### ==++[GitHub](https://github.com/qko03160316/FaceHacker)++==
## 目標： [Face2Face](http://www.graphics.stanford.edu/~niessner/papers/2016/1facetoface/thies2016face.pdf)
{%youtube ohmajJTcpNk %}
<br><br><br>

-------------------------------


## 流程圖
#### 大致上：

```flow
st=>start
frame=>inputoutput: 人臉圖(Frame)
landmarks=>operation: 68個特徵點(Landmarks)
ifenclose=>condition: 得到Enclosing Box?
genRP=>operation: 產生Rendering Parameters
genACM=>operation: 產生Affine Camera Matrix
genMesh=>operation: 產生Mesh(x, y, z)
transform=>operation: Transform(縮放->旋轉->位移)
rendering=>operation: ****OpenGL Rendering*****


st->frame->ifenclose
landmarks->genRP->genACM->genMesh->transform->rendering->frame
ifenclose(yes)->landmarks
ifenclose(no)->frame



```
#### Transform：
```flow
input=>inputoutput: Mesh
anchoring=>operation: 設定錨點為右眼
Mesh.translate(-vertex[181])
scaling=>operation: 縮放
Ratio = (兩眼距離比)*(往前移動之視差大小)
Mesh.scale(Ratio, Ratio, Ratio)
rotating=>operation: 旋轉
Mesh.rotate(r_x, r_y, r_z)
translating=>operation: 移回2D圖的右眼位置
Mesh.translate(worldspace(landmarks[39]))


input->anchoring->scaling->rotating->translating
```

#### genMesh：
```flow
ALLinput=>inputoutput: -Affine Camera Matrix(仿射矩陣),
-sfm_shape_3448.bin(人臉資料庫),
-expression_blendshapes_3448.bin(表情資料庫),
-image_points(特徵點在frame的位置),
-vertex_indices(特徵點mapping到Mesh的第幾個點)

mPb=>operation: Mean Plus Blendshape
fitSL=>subroutine: fit_shape_to_landmarks_linear
(依照特徵點位置和臉部基底, 得到人臉係數)
fitBSL=>subroutine: fit_blendshapes_to_landmarks_linear
(依照特徵點位置和pca model, 得到表情係數)
combine=>operation: 混合
人臉係數*morphable_model
和
表情係數*blend_shapes

cond=>condition: 係數與上一次的係數做比較
平均差距 < 0.5 ?

ret=>inputoutput: return Mesh



ALLinput->mPb->fitSL->fitBSL->combine->cond
cond(yes)->ret
cond(no)->mPb
```


<br>
<br>
<br>


-------------------------------
## 已完成
- [x] 圖片操作 (OpenCV)
- [x] [dlib](http://dlib.net/) 偵測Enclosing Box & Landmarks
- [x] [eos](https://github.com/patrikhuber/eos) 產生 Mesh
- [x] GPU 加速 (OpenGL)
- [x] blendshapes加速(更改精準度)
- [x] 臉部位置對準（似乎沒很準）
- [x] 去[eos](https://github.com/patrikhuber/eos)作者的github 問問題～ [link](https://github.com/patrikhuber/eos/issues/62)

<br>
<br>
<br>

-------------------------------
## 待完成
- [ ]使用 git 合作 
- [ ]穩定特徵點 => 進而穩定Mesh
- [ ]臉部完全貼合 [參考](https://s3-us-west-1.amazonaws.com/disneyresearch/wp-content/uploads/20150722175853/Real-Time-High-Fidelity-Facial-Performance-Capture-Paper1.pdf){%youtube MMa2oT1wMIs %}
- [ ]貼圖速度提升 
- [ ]分析target video
- [ ]source 控制 target 3D Mesh

<br>
<br>
<br>

-------------------------------
## Question
1. 





