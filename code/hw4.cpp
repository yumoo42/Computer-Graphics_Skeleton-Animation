/*

        Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Tutorial 38 - Skinning
*/

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif
#include <sys/types.h>

#include "ogldev_engine_common.h"
#include "ogldev_app.h"
#include "ogldev_camera.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "skinning_technique.h"
#include "ogldev_glut_backend.h"
#include "ogldev_skinned_mesh.h"

using namespace std;

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

class Tutorial38 : public ICallbacks, public OgldevApp {
public:
    Tutorial38() {
        m_pGameCamera = NULL;
        m_pEffect = NULL;
        m_directionalLight.Color = Vector3f(0.5f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = 0.55f;
        m_directionalLight.DiffuseIntensity = 0.9f;
        m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

        m_persProjInfo.FOV = 50.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 200.0f;

        m_position = Vector3f(0.0f, 2.0f, 2.0f);
    }

    ~Tutorial38() {
        SAFE_DELETE(m_pEffect);
        SAFE_DELETE(m_pGameCamera);
    }

    bool Init() {
        Vector3f Pos(0.0f, 2.0f, -5.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, -5.0f);

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

        m_pEffect = new SkinningTechnique();
        if (!m_pEffect->Init()) {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pEffect->Enable();
        m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
        m_pEffect->SetDirectionalLight(m_directionalLight);
        m_pEffect->SetMatSpecularIntensity(0.0f);
        m_pEffect->SetMatSpecularPower(0);

        if (!m_mesh.LoadMesh("../Content/model.dae")) {
            printf("Mesh load failed\n");
            return false;
        }

#ifndef WIN32
        if (!m_fontRenderer.InitFontRenderer()) {
            return false;
        }
#endif
        return true;
    }

    void Run() {
        GLUTBackendRun(this);
    }

    virtual void RenderSceneCB() {
        if (m_mouseActive || m_keyboardActive) {
            m_pGameCamera->OnRender();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_pEffect->Enable();

        vector<Matrix4f> Transforms;

        Pipeline pGlobal;
        pGlobal.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        pGlobal.SetPerspectiveProj(m_persProjInfo);

        Pipeline pModel;
        pModel.SetPerspectiveProj(m_persProjInfo);
        pModel.Scale(0.1f, 0.1f, 0.1f);
        

        float RunningTime = GetRunningTime();

        if(isAnimate || !m_initialTransformSet) {
            float RunningTime = GetRunningTime();
            m_mesh.BoneTransform(RunningTime, Transforms);
            UpdateModelPosition();
            float heading = GetModelHeading();
            pModel.Rotate(270.0f, heading, 360.0f);

            // 儲存當前狀態
            m_lastTime = RunningTime;
            m_lastPosition = m_position;
            m_lastHeading = heading;
            m_initialTransformSet = true;
        } else {
            // 使用儲存的狀態
            pModel.Rotate(270.0f, m_lastHeading, 360.0f);
            m_position = m_lastPosition;
        }

        pModel.WorldPos(m_position);

        for (uint i = 0; i < Transforms.size(); i++) {
            m_pEffect->SetBoneTransform(i, Transforms[i]);
        }

        m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

        m_pEffect->SetWVP(pGlobal.GetVPTrans() * pModel.GetWorldTrans());
        m_pEffect->SetWorldMatrix(pModel.GetWorldTrans());

        m_mesh.Render();

        glutSwapBuffers();
    }

    
    
    virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State) {
        m_keyboardActive = true;
        switch (OgldevKey) {
            case OGLDEV_KEY_q:
                GLUTBackendLeaveMainLoop();
                break;
            case OGLDEV_KEY_s:
                isAnimate = !isAnimate;
                break;
            default:
                m_pGameCamera->OnKeyboard(OgldevKey);
        }
        m_keyboardActive = false;
    }

    virtual void PassiveMouseCB(int x, int y) {
        m_mouseActive = true;
        m_pGameCamera->OnMouse(x, y);
        m_mouseActive = false;
    }

private:
    void UpdateModelPosition() {
        float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        float speedFactor = 0.5f;
        float radius = 3.0f;
        m_position.x = 0.0f + radius * sinf(currentTime * speedFactor);
        m_position.z = 2.0f + radius * cosf(currentTime * speedFactor);
    }
    
    float GetModelHeading() {
        float x = m_position.x;
        float z = m_position.z;

        float dx = x - 0.0f; 
        float dz = z - 2.0f;

        return atan2f(dx, -dz) * 180.0f / M_PI + 180.0f;
    }

    bool isAnimate = false;
    SkinningTechnique* m_pEffect;
    Camera* m_pGameCamera;
    DirectionalLight m_directionalLight;
    SkinnedMesh m_mesh;
    Vector3f m_position;
    PersProjInfo m_persProjInfo;
    Texture* m_pColorMap;
    bool m_mouseActive;
    bool m_keyboardActive;
    float m_lastTime; // 儲存最後一次更新時間
    Vector3f m_lastPosition; // 儲存最後一次模型位置
    float m_lastHeading; // 儲存最後一次模型朝向
    bool m_initialTransformSet = false; // 是否已設置初始變換
};

int main(int argc, char** argv) {
    GLUTBackendInit(argc, argv, true, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "HW4")) {
        return 1;
    }

    SRANDOM;

    Tutorial38* pApp = new Tutorial38();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;

    return 0;
}
