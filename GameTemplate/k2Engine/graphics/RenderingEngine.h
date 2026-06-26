#pragma once

#include "MyRenderer.h"
#include "graphics/preRender/ShadowMapRender.h"
#include "graphics/postEffect/PostEffect.h"
#include "graphics/light/SceneLight.h"
#include "graphics/preRender/LightCulling.h"
#include "geometry/SceneGeometryData.h"
#include "graphics/light/VolumeLightRender.h"
#include "../Game/CompositeBackground.h"

namespace nsK2Engine {
   
    /// <summary>
    /// �����_�����O�G���W���B
    /// </summary>
    /// <remark>
    /// K2Engine�̂P�t���[���̃����_�����O�̗���͎��̂悤�ɂȂ��Ă��܂��B
    /// �P.ComputeAnimatedVertex
    ///     ���[���h��Ԃɕϊ��ς݂̒��_���v�Z����B   
    ///     �ȍ~�̃��f���`��p�X�ł́A���̃p�X�Ōv�Z���ꂽ���_���g���񂷁B
    /// �@�@���̃p�X�ŃC���X�^���V���O�`����s�����f���̒��_�o�b�t�@�͂P�̑傫�ȃo�b�t�@�ɂ܂Ƃ߂���B
    ///     �Ⴆ�΁A���郂�f�����C���X�^���V���O�`���100���_�̃��f����100�̕`�悷��ݒ�ɂȂ��Ă���ꍇ�A
    ///     100���_ �~ 100��1�����_���L���ł��钸�_�o�b�t�@�������B
    ///     ���_�o�b�t�@���܂Ƃ߂闝�R��DXR�̍������̂��߁B
    ///     DXR�ł̓��C�g�����[���h�ɓo�^����I�u�W�F�N�g�̐��������Ȃ�ƁA�����I��BVH���\�z�ł��Ȃ��Ȃ�A
    ///     �傫�����x���ቺ����B���̃I�u�W�F�N�g���P�̒��_�o�b�t�@�ɓZ�߂邱�Ƃő啝�ȍ��������\�ƂȂ�B
    /// 
    /// 2. �V���h�E�}�b�v�̍쐬
    ///     �J�X�P�[�h�V���h�E���𗘗p���ĂS�̃��C�g����̃V���h�E�}�b�v���쐬����B
    ///     ���̂��߁A�S�̃��C�g�@�~�@�R���̃V���h�E�}�b�v(�ߋ����p�A�������p�A�������p)�̍��v12���̃V���h�E�}�b�v���쐬�����B
    /// 
    /// 3. ZPrepass
    ///     ���C�g�J�����O�̂��߂�ZPrepass�B
    ///     ���ʂȃs�N�Z���`�������邽�߂ł͂Ȃ��A�[�x�e�N�X�`�����쐬���邽�߂ɑ��݂��Ă���p�X�B
    ///     ��q����G-Buffer�̐[�x�e�N�X�`�����g���΂����悤�Ɋ����邪�A
    ///     �f�B�t�@�[�h�ƃt�H���[�h�̗����̐[�x�l���������܂ꂽ�e�N�X�`�����K�v���������ߒǉ��B
    /// 
    /// 4. ���C�g�J�����O
    ///     �^�C�����Ƃ̃|�C���g���C�g�ƃX�|�b�g���C�g�̔ԍ����X�g���쐬�B
    /// 
    /// 5. G-Buffer�ւ̃����_�����O
    ///     G-Buffer�̓A���x�h&�[�x�l�A�@���A���^���b�N&�V���h�E&Smooth�̂R���B
    /// 
    /// 6. ���C�g���[�V���O�ŉf�荞�݉摜���쐬����B
    ///     NVIDIA��RTX�V���[�Y��GPU������Ă���ꍇ�A���C�g����p���Ċe�I�u�W�F�N�g�ւ̉f�荞�݉摜�𐶐�����B
    ///     ���̃p�X��NVIDIA��RTX�V���[�Y�ł����ʏ�͎g���Ȃ��B
    ///     �������Ak2EngineLow\k2EngineLowPreCompile.h��ENABLE_DXR_ON_RTX_ONLY�̃}�N���𖳌��ɂ��邱�Ƃ�
    ///     DXR�����p�ł���ꕔ��RTX�ȊO��GPU�iNVIDIA GTX1060�Ȃǁj�ł����C�g�����s����B
    ///     �������AGTX�V���[�Y�ł̓p�t�H�[�}���X���傫�����Ȃ��邽�߁A�����܂ł��J���p�B
    /// 
    /// 7. �f�B�t�@�[�h���C�e�B���O
    ///     �f�B�Y�j�[�x�[�X��PBR���C�e�B���O���s����B
    /// 
    /// 8. �t�H���[�h�����_�����O
    ///     �������I�u�W�F�N�g�����ȃV�F�[�f�B���O�̕`��p�X�B
    /// 
    /// 9. �|�X�g�G�t�F�N�g
    ///     ���L�̃|�X�g�G�t�F�N�g�����s�����B
    ///     9.1 �X�N���[���X�y�[�X���t���N�V����(���C�g�����s���Ă���ꍇ�͏�������Ȃ��B)
    ///     9.2 �V�[���̋P�x�v�Z(��q��HDR�����_�����O�̃u���[����g�[���}�b�v�Ŏg�p�����B)
    ///     9.3 �u���[��
    ///     9.4 �g�[���}�b�v
    ///     9.5 Effekseer�̃G�t�F�N�g�`��
    ///     9.6 DOF
    ///     9.7 FXAA + �K���}�␳
    /// 
    /// 10. 2D�`��
    /// 11. �ŏI�摜���t���[���o�b�t�@�ɃR�s�[�B
    /// </reramk>

    class RenderingEngine : public Noncopyable
    {

    public:
        // �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
        struct SDeferredLightingCB
        {
            Light m_light;              // ���C�g
            Matrix mlvp[MAX_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP]; // ���C�g�r���[�v���W�F�N�V�����s��B
            float m_iblLuminance;       // IBL�̖��邳�B
            int m_isIBL;                // IBL���s���B
            int m_isEnableRaytracing;   // ���C�g�����s���Ă���B
        };
        /// <summary>
        /// ���C�g���p�̃��C�g�f�[�^�B
        /// </summary>
        struct RaytracingLightData {
            DirectionalLight m_directionalLight;  // �f�B���N�V���i�����C�g�B
            Vector3 m_ambientLight;               // �����BIBL�e�N�X�`�����w�肳��Ă��Ȃ��ꍇ�ɗ��p�����B
            float m_iblIntencity;                 // IBL���x�B
            int m_enableIBLTexture;               // IBL�e�N�X�`�����w�肳��Ă���B

        };
        //���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g
        enum class EnMainRTSnapshot
        {
            enDrawnOpacity,     // �s�����I�u�W�F�N�g�̕`�抮�����_
            enNum,              // �X�i�b�v�V���b�g�̐�
        };

        // �����_�����O�p�X
        enum class EnRenderingPass
        {
            enComputeAnimatedVertex,    // �A�j���[�V�����ςݒ��_�v�Z�B
            enRenderToShadowMap,        // �V���h�E�}�b�v�ւ̕`��p�X
            enZPrepass,                 // ZPrepass
            enRenderToVolumeLightMap,   // �{�����[�����C�g�}�b�v�ւ̕`��
            enLightCulling,             // ���C�g�J�����O�B
            enRenderToGBuffer,          // G-Buffer�ւ̕`��p�X
            enForwardRender,            // �t�H���[�h�����_�����O�̕`��p�X
            enPostEffect,               // �|�X�g�G�t�F�N�g
            enRender2D,                 // 2D�`��B
        };
        /// <summary>
        /// �C�x���g�B
        /// </summary>
        enum EnEvent {
            enEventReInitIBLTexture,    // IBL�e�N�X�`�����ď��������ꂽ�B
        };
        RenderingEngine();
        ~RenderingEngine();
        /// <summary>
        /// �����_�����O�p�C�v���C����������
        /// </summary>
        /// <param name="isSoftShadow">
        /// true�̏ꍇ�A�V���h�E�}�b�v�@�ɂ��A�e�������\�t�g�V���h�E�ɂȂ�܂��B
        /// �n�[�h�V���h�E�ɂ������ꍇ�́Afalse���w�肵�Ă��������B
        /// </param>
        void Init(bool isSoftShadow);

        /**
		 * @brief CompositeBackground�̏�Z�J���[��ݒ肷��B
         * @param enable 
         */
        void EnableCompositeBackground(bool enable);

        /**
		 * @brief CompositeBackground�̍X�V�B
         * @param cameraWorldX �J�����̃��[���h��Ԃ�X���W�B
         */
        void UpdateCompositeBackground(float cameraWorldX)
        {
            m_compositeBackground.Update(cameraWorldX);
        }

        /// <summary>
        /// �`��I�u�W�F�N�g��ǉ��B
        /// </summary>
        /// <param name="renderObject"></param>
        void AddRenderObject(IRenderer* renderObject)
        {
            m_renderObjects.push_back(renderObject);
        }
        /// <summary>
        /// ���C�g�����[���h�Ƀ��f����ǉ��B
        /// </summary>
        /// <param name="model">�ǉ����郂�f���B</param>
        void AddModelToRaytracingWorld(Model& model) 
        {
            g_graphicsEngine->RegistModelToRaytracingWorld(model);
        }
        /// <summary>
        /// ���C�g�����[���h�̍č\�z���N�G�X�g�𑗂�B
        /// </summary>
        void RequestRebuildRaytracingWorld()
        {
            g_graphicsEngine->RequestRebuildRaytracingWorld();
        }
        /// <summary>
        /// ���C�g�����[���h���烂�f�����폜�B
        /// </summary>
        /// <param name="model">�폜���郂�f���B</param>
        void RemoveModelFromRaytracingWorld(Model& model) 
        {
            g_graphicsEngine->RemoveModelFromRaytracingWorld(model);
            
        }
        /// <summary>
        /// ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`�����擾
        /// </summary>
        /// <returns></returns>
        Texture& GetZPrepassDepthTexture()
        {
            return m_zprepassRenderTarget.GetRenderTargetTexture();
        }
        /// <summary>
        /// GBuffer�̃A���x�h�e�N�X�`�����擾
        /// </summary>
        /// <returns></returns>
        Texture& GetGBufferAlbedoTexture()
        {
            return m_gBuffer[enGBufferAlbedoDepth].GetRenderTargetTexture();
        }
        /// <summary>
        /// GBuffer�̖@���e�N�X�`�����擾�B
        /// </summary>
        /// <returns></returns>
        Texture& GetGBufferNormalTexture()
        {
            return m_gBuffer[enGBufferNormal].GetRenderTargetTexture();
        }
        /// <summary>
        /// �s�����I�u�W�F�N�g�̕`�抮�����̃��C�������_�����O�^�[�Q�b�g��
        /// �X�i�b�v�V���b�g���擾
        /// </summary>
        /// <returns></returns>
        Texture& GetMainRenderTargetSnapshotDrawnOpacity()
        {
            return m_mainRTSnapshots[(int)EnMainRTSnapshot::enDrawnOpacity].GetRenderTargetTexture();
        }
        /// <summary>
        /// �V�[���̕��ϋP�x�e�N�X�`�����擾�B
        /// </summary>
        /// <returns></returns>o
        Texture& GetLuminanceAvgTextureInScene()
        {
            return m_postEffect.GetLuminanceAvgTextureInScene();
        }
        /// <summary>
        /// �����_�����O�p�C�v���C�������s
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
        void Execute(RenderContext& rc);
        /// <summary>
        /// �f�B���N�V�������C�g�̃p�����[�^��ݒ�
        /// </summary>
        /// <param name="lightNo"></param>
        /// <param name="direction"></param>
        /// <param name="color"></param>
        void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
        {
            m_sceneLight.SetDirectionLight(lightNo, direction, color);
        }
        void SetMainRenderTargetAndDepthStencilBuffer(RenderContext& rc)
        {
            rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_zprepassRenderTarget.GetDSVCpuDescriptorHandle());
        }
        /// <summary>
        /// �\�t�g�V���h�E���s��������B
        /// </summary>
        /// <returns></returns>
        bool IsSoftShadow() const
        {
            return m_isSoftShadow;
        }
        /// <summary>
        /// �e�����̃p�����[�^��ݒ肷��
        /// </summary>
        /// <remark>
        /// �e�����̓J�X�P�[�h�V���h�E�Z�@�ōs���Ă��܂��B
        ///  
        /// </renmark>
        /// <param name=""></param>
        /// <param name="middle"></param>
        /// <param name=""></param>
        /// <param name="lightHeght"></param>
        void SetShadowParameter(float near, float middle, float far, float lightHeght);
        /// <summary>
        /// �J�X�P�[�h�V���h�E�̃G���A����ݒ�B
        /// </summary>
        /// <remark>
        /// �Q�[���J�����̋ߕ��ʂ��牓���ʂ܂ł̃G���A�́A��%�͈̔͂��ߋ����p�̍��𑜓x�̃V���h�E�}�b�v�ɁA
        /// ��%�𒆋����p�̃V���h�E�}�b�v�ɁA��%���������p�̃V���h�E�}�b�v�ɕ`�悷�邩���w�肵�����́B
        /// �Ⴆ�΁AnearArea  = 0.1(10%)�AmiddleArea = 0.3(30%), farArea = 0.8(80%)�Ǝw�肳��Ă�����A
        /// �J��������10%�̃G���A�͋ߋ����p�̃V���h�E�}�b�v�ɕ`��A
        /// �J��������10%�`30%�̃G���A�͒������p�̃V���h�E�}�b�v�ɕ`��A
        /// �J��������30%�`80%�̃G���A�͉������p�̃V���h�E�}�b�v�ɕ`��ƂȂ�B
        /// �e�̕i���̒����Ɏg���Ă��������B
        /// </remark>
        /// <param name="nearArea">�߉e�G���A���B0.0f�`1.0f</param>
        /// <param name="nearArea">���e�G���A���B0.0f�`1.0f</param>
        /// <param name="nearArea">���e�G���A���B0.0f�`1.0f</param>
        void  SetCascadeNearAreaRates(float nearArea, float middleArea, float farArea)
        {
            for (auto& renderer : m_shadowMapRenders) {
                renderer.SetCascadeNearAreaRates(nearArea, middleArea, farArea);
            }
        }
        /// <summary>
        /// �r���[�J�����O�̂��߂̃r���[�v���W�F�N�V�����s����擾�B
        /// </summary>
        /// <returns></returns>
        const Matrix& GetViewProjectionMatrixForViewCulling() const
        {
            return m_viewProjMatrixForViewCulling;
        }
        /// <summary>
        /// �V�[�����؂�ւ�������Ƃ�ʒm����B
        /// </summary>
        /// <remark>
        /// �V�[���̘A�����𗘗p�����O���t�B�b�N�����̂��߂̊֐��ł��B
        /// ���Ï����ATAA���̏����́A�V�[�����؂�ւ�����Ƃ��ɂ��̊֐����Ăяo���Ȃ���
        /// ���t���[���̊ԁA�\�����������������Ȃ�܂��B
        /// </remark>
        /// <param name="changeSceneTime">�V�[���؂�ւ��ɂ����鎞�ԁB</param>
        void NotifyChangeScene(float changeSceneTime)
        {
            m_postEffect.NotifyChangeScene(changeSceneTime);
        }
        /// <summary>
        /// �􉽊w�f�[�^��o�^
        /// </summary>
        /// <param name="geomData">�􉽊w�f�[�^</param>
        void RegisterGeometryData(GemometryData* geomData)
        {
            m_sceneGeometryData.RegisterGeometryData(geomData);
        }
        /// <summary>
        /// �􉽊w�f�[�^�̓o�^�����B
        /// </summary>
        /// <param name="geomData"></param>
        void UnregisterGeometryData(GemometryData* geomData)
        {
            m_sceneGeometryData.UnregisterGeometryData(geomData);
        }
        /// <summary>
        /// �X�V�B
        /// </summary>
        void Update();
        /// <summary>
        /// �C�x���g���X�i�[��ǉ��B
        /// </summary>
        void AddEventListener(
            void* pListenerObj,
            std::function<void(EnEvent enEvent)> listenerFunc
        )
        {
            m_eventListeners.push_back({ pListenerObj, listenerFunc });
        }
        /// <summary>
        /// �C�x���g���X�i�[���폜�B
        /// </summary>
        void RemoveEventListener(void* pListenerObj)
        {
            auto it = std::find_if(
                m_eventListeners.begin(),
                m_eventListeners.end(),
                [&](const SEventListenerData& listenerData) {return listenerData.pListenerObj == pListenerObj; }
            );
            if (it != m_eventListeners.end()) {
                m_eventListeners.erase(it);
            }
        }
        /// <summary>
        /// IBL���ď������B
        /// </summary>
        void ReInitIBL(const wchar_t* iblTexFilePath, float luminance);
        /// <summary>
        /// IBL�e�N�X�`�����擾�B
        /// </summary>
        /// <returns></returns>
        Texture& GetIBLTexture()
        {
            return m_iblData.m_texture;
        }
        /// <summary>
        /// �V���h�E�}�b�v�e�N�X�`���ɃN�G�����s���B
        /// </summary>
        /// <param name="queryFunc">�N�G���֐�</param>
        void QueryShadowMapTexture(std::function< void(Texture& shadowMap) > queryFunc)
        {
            for (int i = 0; i < MAX_DIRECTIONAL_LIGHT; i++)
            {
                for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
                {
                    queryFunc(m_shadowMapRenders[i].GetShadowMap(areaNo));
                }
            }
        }
        SDeferredLightingCB& GetDeferredLightingCB()
        {
            return m_deferredLightingCB;
        }
        /// <summary>
        /// �g�[���}�b�v�𖳌��ɂ���
        /// </summary>
        void DisableTonemap()
        {
            m_postEffect.DisableTonemap();
        }
        /// <summary>
        /// �g�[���}�b�v��L���ɂ���B
        /// </summary>
        void EnableTonemap()
        {
            m_postEffect.EnableTonemap();
        }
        /// <summary>
        /// �g�[���}�b�v���L�������肷��B
        /// </summary>
        /// <returns></returns>
        bool IsEnableTonemap() const
        {
            return m_postEffect.IsEnableTonemap();
        }
        /// <summary>
        /// �V�[���̒��Ԃ̖��邳���������x�����w�肷��B
        /// </summary>
        /// <remark>
        /// ���̐ݒ�̓|�X�g�G�t�F�N�g�̃g�[���}�b�v���L���ȂƂ��ɗ��p����܂��B
        /// ���K�I��0.18���g���Ă���B
        /// https://en.wikipedia.org/wiki/Middle_gray
        /// </remark>
        void SetSceneMiddleGray(float luminance)
        {
            m_postEffect.SetTonemapMiddlegray(luminance);
        }
        /// <summary>
        /// �V�[���̒��Ԃ̖��邳���������x�����擾����B
        /// </summary>
        /// <returns></returns>
        float GetSceneMiddleGray() const
        {
            return m_postEffect.GetTonemapMiddlegray();
        }
        /// <summary>
        /// �u���[������������臒l��ݒ�B
        /// </summary>
        /// <param name="value"></param>
        void SetBloomThreshold(float value)
        {
            m_postEffect.SetBloomThreshold(value);
        }
        /// <summary>
        /// �u���[������������臒l���擾�B
        /// </summary>
        /// <returns></returns>
        float GetBloomThreshold() const
        {
            return m_postEffect.GetBloomThreshold();
        }
        /// <summary>
        /// �{�����[���X�|�b�g���C�g���V�[���ɒǉ�
        /// </summary>
        /// <param name="lig">���C�g</param>
        void AddVolumeSpotLight(VolumeLightBase& lig)
        {
            m_volumeLightRender.AddVolumeSpotLight(lig);
        }
        /// <summary>
        /// �{�����[���X�|�b�g���C�g���V�[������폜
        /// </summary>
        /// <param name="lig"></param>
        void RemoveVolumeSpotLight(VolumeLightBase& lig)
        {
            m_volumeLightRender.RemoveVolumeSpotLight(lig);
        }
        /// <summary>
        /// �{�����[�����C�g�����_���[���擾�B
        /// </summary>
        /// <returns></returns>
        VolumeLightRender& GetVolumeLightRender()
        {
            return m_volumeLightRender;
        }
        /// <summary>
        /// �����̌v�Z�̂��߂�IBL�e�N�X�`����ݒ�B
        /// </summary>
        /// <remark>
        /// ���̊֐��𗘗p���āAIBL�e�N�X�`�����Z�b�g����ƁA
        /// ������IBL�e�N�X�`������T���v�����O���āA����𗘗p����
        /// ���C�e�B���O���s���܂��B
        /// IBL�e�N�X�`���𗘗p���������̌v�Z���I�t�ɂ������ꍇ�́ADisableIBLForAmbinet()���Ăяo���āA
        /// IBL�𖳌��ɂ��Ă��������B
        /// </remark>
        /// <param name="textureFilePath">
        /// IBL�e�N�X�`���̃t�@�C���p�X�B
        /// �L���[�u�}�b�v�ł���K�v������܂��B
        /// </param>
        /// <param name="luminance">
        /// IBL�e�N�X�`���̖��邳�B
        /// <param>
        void SetAmbientByIBLTexture(const wchar_t* textureFilePath, float luminance)
        {
            ReInitIBL(textureFilePath, luminance);
        }
        /// <summary>
        /// IBL�����𖳌��ɂ���B
        /// </summary>
        void DisableIBLTextureForAmbient()
        {
            m_sceneLight.DisableIBLTextureForAmbient();
        }
        /// <summary>
        /// ������ݒ�B
        /// </summary>
        /// <param name="ambient"></param>
        void SetAmbient(Vector3 ambient)
        {
            m_sceneLight.SetAmbinet(ambient);
        }
        /// <summary>
        /// ���C�g���[�V���O���L�����ǂ����𔻒肷��B
        /// </summary>
        /// <returns></returns>
        bool IsEnableRaytracing() const
        {
            return m_isEnableRaytracing && g_graphicsEngine->IsPossibleRaytracing();
        }
        /// <summary>
        /// ���C�g���[�V���O��L���ɂ��܂��B
        /// ���̐ݒ�̓n�[�h�E�F�A���C�g���[�V���O�������ȏꍇ�͖�������܂��B
        /// </summary>
        void EnableRaytracing()
        {
            m_isEnableRaytracing = true && IsEnableRaytracing();
        }
        /// <summary>
        /// ���C�g���[�V���O�𖳌��ɂ��܂��B
        /// ���̐ݒ�̓n�[�h�E�F�A���C�g���[�V���O�������ȏꍇ�͖�������܂��B
        /// </summary>
        void DisableRaytracing()
        {
            m_isEnableRaytracing = false && IsEnableRaytracing();
        }
        /// <summary>
        /// ���C�g���p�̃��C�g�f�[�^���擾�B
        /// </summary>
        /// <returns></returns>
        RaytracingLightData& GetRaytracingLightData()
        {
            return m_raytracingLightData;
        }
    private:
        /// <summary>
        /// �C���[�W�x�[�X�h���C�e�B���O(IBL)�̂��߂̃f�[�^������������B
        /// </summary>
        /// <param name="iblTexFilePath">IBL�e�N�X�`���̃t�@�C���p�X�B</param>
        /// <param name="intencity">IBL�̋��x�B</param>
        void InitIBLData(const wchar_t* iblTexFilePath, float intencity);
        /// <summary>
        /// G-Buffer��������
        /// </summary>
        void InitGBuffer();
        /// <summary>
        /// �f�B�t�@�[�h���C�e�B���O�̏�����
        /// </summary>
        void InitDeferredLighting();
        /// <summary>
        /// �f�B�t�@�[�h���C�e�B���O�Ŏg�p����X�v���C�g���������B
        /// </summary>
        void InitDefferedLighting_Sprite();
        /// <summary>
        /// �A�j���[�V�����ςݒ��_�̌v�Z�B
        /// </summary>
        /// <remark>
        /// </remark>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        void ComputeAnimatedVertex(RenderContext& rc);
        /// <summary>
        /// �V���h�E�}�b�v�ɕ`��
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        void RenderToShadowMap(RenderContext& rc);
        /// <summary>
        /// ZPrepass
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        void ZPrepass(RenderContext& rc);
        /// <summary>
        /// G-Buffer�ւ̕`��
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
        void RenderToGBuffer(RenderContext& rc);
        /// <summary>
        /// �f�B�t�@�[�h���C�e�B���O
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        void DeferredLighting(RenderContext& rc);
        /// <summary>
        /// 2D�`��
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        void Render2D(RenderContext& rc);
        /// <summary>
        /// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
        /// <summary>
        /// �t�H���[�h�����_�����O
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        void ForwardRendering(RenderContext& rc);
        /// <summary>
        /// ���C�������_�����O�^�[�Q�b�g��������
        /// </summary>
        void InitMainRenderTarget();
        /// <summary>
        /// ���C�������_�����O�^�[�Q�b�g�̂̃X�i�b�v�V���b�g����邽�߂�
        /// �����_�����O�^�[�Q�b�g��������
        /// </summary>
        void InitMainRTSnapshotRenderTarget();
        /// <summary>
        /// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓��e��
        /// �t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g������������
        /// </summary>
        void InitCopyMainRenderTargetToFrameBufferSprite();
        /// <summary>
        /// ZPrepass�p�̃����_�����O�^�[�Q�b�g��������
        /// </summary>
        void InitZPrepassRenderTarget();
        /// <summary>
        /// ���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g���B�e
        /// </summary>
        void SnapshotMainRenderTarget(RenderContext& rc, EnMainRTSnapshot enSnapshot);
        /// <summary>
        /// �V���h�E�}�b�v�ւ̕`�揈����������
        /// </summary>
        void InitShadowMapRender();
        /// <summary>
        /// �r���[�J�����O�p�̃r���[�v���W�F�N�V�����s����v�Z�B
        /// </summary>
        void CalcViewProjectionMatrixForViewCulling();
        /// <summary>
        /// 2D�`��p�̃����_�\�^�[�Q�b�g��������
        /// </summary>
        void Init2DRenderTarget();
    private:
        // GBuffer�̒�`
        enum EnGBuffer
        {
            enGBufferAlbedoDepth,           // �A���x�h�Ɛ[�x�l�B���ɐ[�x�l���L������Ă��܂��B
            enGBufferNormal,                // �@��
            enGBufferMetaricShadowSmooth,   // ���^���b�N�A�e�p�����[�^�A�X���[�X�B
                                            // ���^���b�N��r�A�e�p�����[�^��g�A�X���[�X��a�Bg�͖��g�p�B
                                            enGBufferNum,                   // G-Buffer�̐�
        };
        /// <summary>
        /// IBL�f�[�^
        /// </summary>
        struct SIBLData {
            Texture m_texture;          // IBL�e�N�X�`��
            float m_intencity = 1.0f;   // ���x�B
        };
        
        /// <summary>
        /// GI�e�N�X�`������邽�߂̃u���[�����B
        /// </summary>
        enum EGITextureBlur {
            eGITextureBlur_1024x1024,   // 1024�~1024
            eGITextureBlur_512x512,     // 512�~512
            eGITextureBlur_256x256,     // 256�~256
            eGITextureBlur_128x128,     // 128�~128
            eGITextureBlur_Num,     
        };
        RaytracingLightData m_raytracingLightData;                      // ���C�g���p�̃��C�g�f�[�^�B
        LightCulling m_lightCulling;                                    // ���C�g�J�����O�B 
        ShadowMapRender m_shadowMapRenders[MAX_DIRECTIONAL_LIGHT];      // �V���h�E�}�b�v�ւ̕`�揈��
        VolumeLightRender m_volumeLightRender;                          // �{�����[�����C�g�����_���[�B
        SDeferredLightingCB m_deferredLightingCB;                       // �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
        Sprite m_copyMainRtToFrameBufferSprite;                         // ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
        Sprite m_diferredLightingSprite;                                // �f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g
        RenderTarget m_zprepassRenderTarget;                            // ZPrepass�`��p�̃����_�����O�^�[�Q�b�g
        RenderTarget m_mainRenderTarget;                                // ���C�������_�����O�^�[�Q�b�g
        RenderTarget m_mainRTSnapshots[(int)EnMainRTSnapshot::enNum];   // ���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g
        RenderTarget m_gBuffer[enGBufferNum];                           // G-Buffer
        PostEffect m_postEffect;                                        // �|�X�g�G�t�F�N�g
        RWStructuredBuffer m_pointLightNoListInTileUAV;                 // �^�C�����Ƃ̃|�C���g���C�g�̃��X�g��UAV�B
        RWStructuredBuffer m_spotLightNoListInTileUAV;                  // �^�C�����Ƃ̃X�|�b�g���C�g�̃��X�g��UAV�B
        std::vector< IRenderer* > m_renderObjects;                      // �`��I�u�W�F�N�g�̃��X�g�B
        SceneLight m_sceneLight;                                        // �V�[�����C�g�B
        bool m_isSoftShadow = false;                                    // �\�t�g�V���h�E�t���O�B
        Matrix m_viewProjMatrixForViewCulling;                          // �r���[�J�����O�p�̃r���[�v���W�F�N�V�����s��B
        SceneGeometryData m_sceneGeometryData;                          // �V�[���̃W�I���g�����B
        static RenderingEngine* m_instance;		                        // �B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B
        RenderTarget m_2DRenderTarget;                                  // 2D�`��p�̃����_�\�^�[�Q�b�g�B
        Sprite m_2DSprite;                                              // 2D�����p�̃X�v���C�g�B
        Sprite m_mainSprite;
        SIBLData m_iblData;                                             // IBL�f�[�^�B
        bool m_isEnableRaytracing = true;                               // ���C�g���[�V���O���L���H
        GaussianBlur m_giTextureBlur[eGITextureBlur_Num];                                // GI�e�N�X�`���Ƀu���[�������鏈���B
        CompositeBackground m_compositeBackground;


        /// <summary>
        /// �C�x���g���X�i�[�̃f�[�^�B
        /// </summary>
        struct SEventListenerData {
            void* pListenerObj;     // ���X�i�[�I�u�W�F�N�g
            std::function<void(EnEvent enEvent)> listenerFunc;
        };
        
        std::list< SEventListenerData > m_eventListeners;                // �C�x���g���X�i�[�B
    };    
}