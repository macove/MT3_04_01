#include <Novice.h>
#include "GeometryUtility.h"
#include <imgui.h>
const char kWindowTitle[] = "学籍番号";

GeometryUtility geometryUtility;

struct  Ball
{
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
	float radius;
	unsigned int color;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate{ 0.0f,0.0f,0.0f };

	Vector3 translate{ 0.0f,0.0f,0.0f };


	int kWindowWidth = 1280;
	int kWindowHeight = 720;

	float radius = 10.0f;
	float theta = -3.46f;
	float phi = 1.5f;

	float angularVelocity = 3.14f; // π rad/s
	float angle = 0.0f;
	float deltaTime = 1.0f / 60.0f; // Assuming 60 FPS
	float spinRadius = 0.8f;

	Ball ball = { {radius, 0.0f, 0.0f}, {}, {}, 2.0f, 0.05f, 0xFFFFFFFF };

	bool isMoving = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		if (isMoving) {
			angle += angularVelocity * deltaTime;
			
		}
		ball.position.x = cos(angle) * spinRadius;
		ball.position.y = sin(angle) * spinRadius;

		Vector3 cameraPosition = geometryUtility.SphericalToCartesian(radius, theta, phi);
		Vector3 cameraTarget = { 0.0f, 0.0f, 0.0f };
		Vector3 cameraUp = { 0.0f, 1.0f, 0.0f };

		Matrix4x4 viewMatrix = geometryUtility.MakeLookAtMatrix4x4(cameraPosition, cameraTarget, cameraUp);
		Matrix4x4 worldMatrix = geometryUtility.MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 projectionMatrix = geometryUtility.MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = geometryUtility.Multiply(worldMatrix, geometryUtility.Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewProjectionMatrix = geometryUtility.Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = geometryUtility.MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		geometryUtility.DrawGrid(viewProjectionMatrix, viewportMatrix);

		geometryUtility.DrawSphere({ ball.position,0.1f }, viewProjectionMatrix, viewportMatrix, ball.color);

		ImGui::Begin("Window");
		ImGui::DragFloat("theta", &theta, 0.01f);
		ImGui::DragFloat("phi", &phi, 0.01f);
		ImGui::Text("Uniform Circular Motion");
		ImGui::DragFloat("Angular Velocity", &angularVelocity, 0.01f);
		if (ImGui::Button("Start")) {
			isMoving = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			isMoving = false;
		}
		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
