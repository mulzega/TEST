/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mulzega <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 11:27:34 by mulzega           #+#    #+#             */
/*   Updated: 2017/05/22 11:27:36 by mulzega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_init(t_env *e)
{
	WIDTH_WIN = 512;
	HEIGHT_WIN = 384;
	MLX = mlx_init();
	WIN = mlx_new_window(MLX, WIDTH_WIN, HEIGHT_WIN, "Wolf3D_42");
	IMG = mlx_new_image(MLX, WIDTH_WIN, HEIGHT_WIN);
	DATA = mlx_get_data_addr(IMG, &BPP, &SIZELINE, &ENDIAN);
	return (0);
}

void	ft_launch(t_env *e)
{
	mlx_hook(WIN, 2, KeyPressMask, ft_key_hook, e);
	mlx_hook(WIN, 6, 2, mouse_motion, e);
	mlx_hook(WIN, 4, ButtonPressMask, ft_mouse_hook, e);
	mlx_put_image_to_window(MLX, WIN, IMG, 0, 0);
	mlx_loop(MLX);
}

int   main(int ac, char **av)
{
	int		x;
	int		w;
	double posX = 22, posY = 12;  //x and y start position
  double dirX = -1, dirY = 0; //initial direction vector
  double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane
  double time = 0; //time of current frame
  double oldTime = 0; //time of previous frame
	ft_init(&e);
	ft_launch(&e);
	x = 0;
	while(x++ < w)
	{
		double cameraX = 2 * x / double(w) - 1;
    double rayPosX = posX;
    double rayPosY = posY;
    double rayDirX = dirX + planeX * cameraX;
    double rayDirY = dirY + planeY * cameraX;
	}
	//which box of the map we're in
  int mapX = int(rayPosX);
  int mapY = int(rayPosY);

  //length of ray from current position to next x or y-side
  double sideDistX;
  double sideDistY;

   //length of ray from one x or y-side to next x or y-side
  double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
  double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
  double perpWallDist;

  //what direction to step in x or y-direction (either +1 or -1)
  int stepX;
  int stepY;

  int hit = 0; //was there a wall hit?
  int side; //was a NS or a EW wall hit?
	if (rayDirX < 0)
  {
    stepX = -1;
    sideDistX = (rayPosX - mapX) * deltaDistX;
  }
  else
  {
    stepX = 1;
    sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
  }
  if (rayDirY < 0)
  {
    stepY = -1;
    sideDistY = (rayPosY - mapY) * deltaDistY;
  }
  else
  {
    stepY = 1;
    sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
  }
	//perform DDA
	while (hit == 0)
	{
		//jump to next map square, OR in x-direction, OR in y-direction
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			mapX += stepX;
			side = 0;
		}
		else
		{
			sideDistY += deltaDistY;
			mapY += stepY;
			side = 1;
		}
		//Check if ray has hit a wall
		if (worldMap[mapX][mapY] > 0) hit = 1;
	}
	//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
	if (side == 0) perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
	else           perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;
	//Calculate height of line to draw on screen
	int lineHeight = (int)(h / perpWallDist);

	//calculate lowest and highest pixel to fill in current stripe
	int drawStart = -lineHeight / 2 + h / 2;
	if(drawStart < 0)drawStart = 0;
	int drawEnd = lineHeight / 2 + h / 2;
	if(drawEnd >= h)drawEnd = h - 1;
	//choose wall color
	ColorRGB color;
	switch(worldMap[mapX][mapY])
	{
	  case 1:  color = RGB_Red;  break; //red
	  case 2:  color = RGB_Green;  break; //green
	  case 3:  color = RGB_Blue;   break; //blue
	  case 4:  color = RGB_White;  break; //white
	  default: color = RGB_Yellow; break; //yellow
	}

	//give x and y sides different brightness
	if (side == 1) {color = color / 2;}

	//draw the pixels of the stripe as a vertical line
	verLine(x, drawStart, drawEnd, color);
	}

	return (0);
}
