Renderer::ParticleSystem rightLaser;
Renderer::ParticleSystem leftLaser;
Renderer::ParticleSystem middleLaser;

rightLaser.one_size = true;
rightLaser.alive_time = {3, 3};
rightLaser.position_x = {0.2, 0.3};
rightLaser.position_y = {-0.15, -0.1};
rightLaser.velocity_dir = {0, 2*PI};
rightLaser.spawn_size = {0.5, 1.0};

leftLaser.one_size = true;
leftLaser.alive_time = {3, 3};
leftLaser.position_x = {-0.3, -0.2};
leftLaser.position_y = {-0.15, -0.1};
leftLaser.velocity_dir = {0, 2*PI};
leftLaser.spawn_size = {0.5, 1.0};

middleLaser.one_size = true;
middleLaser.alive_time = {3, 3};
middleLaser.position_x = {-0.1, 0.1};
middleLaser.position_y = {0.2, 0.3};
middleLaser.velocity_dir = {0, 2*PI};
middleLaser.spawn_size = {0.7, 1.3};
