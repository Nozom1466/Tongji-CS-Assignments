from django.db import models

# Create your models here.
# Tutorial: https://blog.csdn.net/alwaysbefine/article/details/106086542


class Test(models.Model):
    """
    Test class for django models
    """
    number = models.IntegerField()
    la = models.DecimalField(decimal_places=7, max_digits=10)
    time = models.DateTimeField()
    msg = models.CharField(max_length=200)


class RawData(models.Model):
    MARIS_sample_ID = models.IntegerField(null=True)
    sample_type = models.CharField(max_length=255, null=True)  # Sample type
    laboratory = models.CharField(max_length=255, null=True)  # Laboratory
    latitude = models.DecimalField(max_digits=10, decimal_places=7, null=True)  # Latitude (decimal)
    longitude = models.DecimalField(max_digits=10, decimal_places=7, null=True)  # Longitude (decimal)
    station_id = models.CharField(max_length=255, null=True)  # Station ID
    data_provider_sample_id = models.CharField(max_length=255,null=True)  # Data provider's sample ID
    maris_profile_id = models.CharField(max_length=255, null=True)  # MARIS Profile ID
    maris_transect_id = models.CharField(max_length=255, null=True)  # MARIS Transect ID
    sampling_depth = models.FloatField(default=0.0, null=True)  # Sampling depth (m)
    total_depth = models.FloatField(null=True)  # Total depth (m)
    sampling_start_datetime = models.DateTimeField(null=True)  # Sampling start datetime
    sampling_end_datetime = models.DateTimeField(null=True)  # Sampling end datetime
    nuclide = models.CharField(max_length=255, null=True)  # Nuclide
    value_type = models.CharField(max_length=255, null=True)  # Value type
    activity_or_mda = models.FloatField(default=0.0, null=True)  # Activity or MDA
    uncertainty = models.FloatField(default=0.0, null=True)  # Uncertainty
    unit = models.CharField(max_length=255, null=True)  # Unit
    variable_type = models.CharField(max_length=255, null=True)  # Variable type
    number_of_data_points = models.IntegerField(null=True)  # Number of data points
    minimum_value = models.FloatField(null=True)  # Minimum value
    maximum_value = models.FloatField(null=True)  # Maximum value
    taxon_name = models.CharField(max_length=255, null=True)  # Taxon name
    reported_taxon_name = models.CharField(max_length=255, null=True)  # Reported taxon name
    taxon_rank = models.CharField(max_length=255, null=True)  # Taxon rank
    biogroup = models.CharField(max_length=255, null=True)  # Biogroup
    taxon_database_url = models.URLField(null=True)  # Taxon database URL
    body_part = models.CharField(max_length=255, null=True)  # Body part
    top_of_core_interval = models.FloatField(null=True)  # Top of core interval (cm)
    bottom_of_core_interval = models.FloatField(null=True)  # Bottom of core interval (cm)
    sediment_type = models.CharField(max_length=255, null=True)  # Sediment type
    reported_sediment_type = models.CharField(max_length=255, null=True)  # Reported sediment type
    sample_volume = models.FloatField(null=True)  # Sample volume (L)
    salinity = models.FloatField(null=True)  # Salinity
    temperature = models.FloatField(null=True)  # Temperature [°C]
    filtered = models.CharField(max_length=255, null=True)  # Filtered
    mesh_size = models.CharField(max_length=255, null=True)  # Mesh size (µm)
    sample_surface_area = models.FloatField(null=True)  # Sample surface area (cm2)
    dry_weight = models.FloatField(null=True)  # Dry weight (g)
    wet_weight = models.FloatField(null=True)  # Wet weight (g)
    dry_wet_percentage = models.FloatField(null=True)  # Dry/Wet (%)
    sampling_method = models.CharField(max_length=255, null=True)  # Sampling method
    preparation_method = models.CharField(max_length=255, null=True)  # Preparation method
    drying_method = models.CharField(max_length=255, null=True)  # Drying method
    counting_method = models.CharField(max_length=255, null=True)  # Counting method
    maris_reference_id = models.CharField(max_length=255, null=True)  # MARIS reference ID
    reference_citation = models.TextField(null=True)  # Reference/Citation
    zotero_url = models.URLField(null=True)  # Zotero URL
    reference_note = models.TextField(null=True)  # Reference note
    sample_note = models.TextField(null=True)  # Sample note
    measurement_note = models.TextField(null=True)  # Measurement note
    activity_corrected = models.CharField(max_length=255, null=True)  # Activity corrected
    uncertainty_corrected = models.CharField(max_length=255, null=True)  # Uncertainty corrected
    unit_corrected = models.CharField(max_length=255, null=True)  # Unit corrected


class RadioactiveMaterial(models.Model):
    """
    Radioactive Substance Table
    """
    rd_sub_id = models.IntegerField(unique=True)
    rd_entity_id = models.AutoField(primary_key=True)
    rd_vol = models.FloatField(default=0.0)
    name = models.CharField(max_length=50, unique=True)
    half_life = models.IntegerField(unique=True)
    RAY_TYPES = (
        (0, 'Alpha'),
        (1, 'Beta'),
        (2, 'Gamma'),
    )
    ray_type = models.IntegerField(choices=RAY_TYPES)
    sign = models.CharField(max_length=50, unique=True)
    danger = models.FloatField(default=0.0)

    def __str__(self):
        return self.name


class RadioactiveSource(models.Model):
    """
    Radioactive sources Table
    """
    time = models.DateTimeField(unique=False)
    source_name = models.CharField(max_length=250)
    nuclide = models.CharField(max_length=250)
    lg = models.FloatField()
    at = models.FloatField()
    status = models.CharField(max_length=250, default='NORMAL')
    ISO_country = models.IntegerField()

# ehh
class OceanCurrent(models.Model):
    MARIS_sample_ID = models.IntegerField(null=True)
    sample_type = models.CharField(max_length=255, null=True)  # Sample type
    laboratory = models.CharField(max_length=255, null=True)  # Laboratory
    latitude = models.DecimalField(max_digits=10, decimal_places=7, null=True)  # Latitude (decimal)
    longitude = models.DecimalField(max_digits=10, decimal_places=7, null=True)  # Longitude (decimal)
    station_id = models.CharField(max_length=255, null=True)  # Station ID
    data_provider_sample_id = models.CharField(max_length=255,null=True)  # Data provider's sample ID
    maris_profile_id = models.CharField(max_length=255, null=True)  # MARIS Profile ID
    maris_transect_id = models.CharField(max_length=255, null=True)  # MARIS Transect ID
    sampling_depth = models.FloatField(default=0.0, null=True)  # Sampling depth (m)
    total_depth = models.FloatField(null=True)  # Total depth (m)
    sampling_start_datetime = models.DateTimeField(null=True)  # Sampling start datetime
    sampling_end_datetime = models.DateTimeField(null=True)  # Sampling end datetime
    nuclide = models.CharField(max_length=255, null=True)  # Nuclide
    value_type = models.CharField(max_length=255, null=True)  # Value type
    activity_or_mda = models.FloatField(default=0.0, null=True)  # Activity or MDA
    uncertainty = models.FloatField(default=0.0, null=True)  # Uncertainty
    unit = models.CharField(max_length=255, null=True)  # Unit
    variable_type = models.CharField(max_length=255, null=True)  # Variable type
    number_of_data_points = models.IntegerField(null=True)  # Number of data points
    minimum_value = models.FloatField(null=True)  # Minimum value
    maximum_value = models.FloatField(null=True)  # Maximum value
    taxon_name = models.CharField(max_length=255, null=True)  # Taxon name
    reported_taxon_name = models.CharField(max_length=255, null=True)  # Reported taxon name
    taxon_rank = models.CharField(max_length=255, null=True)  # Taxon rank
    biogroup = models.CharField(max_length=255, null=True)  # Biogroup
    taxon_database_url = models.URLField(null=True)  # Taxon database URL
    body_part = models.CharField(max_length=255, null=True)  # Body part
    top_of_core_interval = models.FloatField(null=True)  # Top of core interval (cm)
    bottom_of_core_interval = models.FloatField(null=True)  # Bottom of core interval (cm)
    sediment_type = models.CharField(max_length=255, null=True)  # Sediment type
    reported_sediment_type = models.CharField(max_length=255, null=True)  # Reported sediment type
    sample_volume = models.FloatField(null=True)  # Sample volume (L)
    salinity = models.FloatField(null=True)  # Salinity
    temperature = models.FloatField(null=True)  # Temperature [°C]
    filtered = models.CharField(max_length=255, null=True)  # Filtered
    mesh_size = models.CharField(max_length=255, null=True)  # Mesh size (µm)
    sample_surface_area = models.FloatField(null=True)  # Sample surface area (cm2)
    dry_weight = models.FloatField(null=True)  # Dry weight (g)
    wet_weight = models.FloatField(null=True)  # Wet weight (g)
    dry_wet_percentage = models.FloatField(null=True)  # Dry/Wet (%)
    sampling_method = models.CharField(max_length=255, null=True)  # Sampling method
    preparation_method = models.CharField(max_length=255, null=True)  # Preparation method
    drying_method = models.CharField(max_length=255, null=True)  # Drying method
    counting_method = models.CharField(max_length=255, null=True)  # Counting method
    maris_reference_id = models.CharField(max_length=255, null=True)  # MARIS reference ID
    reference_citation = models.TextField(null=True)  # Reference/Citation
    zotero_url = models.URLField(null=True)  # Zotero URL
    reference_note = models.TextField(null=True)  # Reference note
    sample_note = models.TextField(null=True)  # Sample note
    measurement_note = models.TextField(null=True)  # Measurement note
    activity_corrected = models.CharField(max_length=255, null=True)  # Activity corrected
    uncertainty_corrected = models.CharField(max_length=255, null=True)  # Uncertainty corrected
    unit_corrected = models.CharField(max_length=255, null=True)  # Unit corrected


class RadiationMonitoringStation(models.Model):
    """
    Radiation monitoring stations table.
    """

    station_id = models.AutoField(primary_key=True)
    desc = models.CharField(max_length=50, null=True)
    lg = models.FloatField()
    at = models.FloatField()
    depth = models.FloatField(null=True)

    def __str__(self):
        """String representation of the radiation monitoring station."""
        return f"Station {self.station_id}"


class RadiationSample(models.Model):
    """
    Radiation samples table
    """

    rd_sub_id = models.AutoField(primary_key=True)
    lg = models.FloatField()
    at = models.FloatField()
    time = models.DateTimeField()
    station_id = models.ForeignKey(RadiationMonitoringStation, on_delete=models.CASCADE)

    def __str__(self):
        """String representation of the radiation sample."""
        return f"Sample {self.rd_sub_id}"


# ehh
class BiologicalSample(models.Model):
    MARIS_sample_id = models.CharField(max_length=250, null=True)
    sample_type = models.CharField(max_length=250, null=True)
    laboratory = models.CharField(max_length=100, null=True)
    latitude = models.DecimalField(max_digits=9, decimal_places=6)
    longitude = models.DecimalField(max_digits=9, decimal_places=6, null=True)
    station_id = models.CharField(max_length=250, null=True)
    data_provider_sample_id = models.CharField(max_length=255,null=True)  # Data provider's sample ID
    maris_profile_id = models.CharField(max_length=250, null=True)
    maris_transect_id = models.CharField(max_length=250, null=True)
    sampling_depth = models.FloatField(null=True)
    total_depth = models.FloatField(null=True)
    sampling_start_datetime = models.DateTimeField(null=True)
    sampling_end_datetime = models.DateTimeField(null=True)
    nuclide = models.CharField(max_length=250, null=True)
    value_type = models.CharField(max_length=250, null=True)
    activity_or_mda = models.FloatField(null=True)
    uncertainty = models.FloatField(null=True)
    unit = models.CharField(max_length=250, null=True)
    variable_type = models.CharField(max_length=250, null=True)
    num_data_points = models.IntegerField(null=True)
    min_value = models.FloatField(null=True)
    max_value = models.FloatField(null=True)
    taxon_name = models.CharField(max_length=100, null=True)
    reported_taxon_name = models.CharField(max_length=100, null=True)
    taxon_rank = models.CharField(max_length=250, null=True)
    biogroup = models.CharField(max_length=250, null=True)
    taxon_database_url = models.URLField(null=True)
    body_part = models.CharField(max_length=250, null=True)
    top_of_core_interval = models.FloatField(null=True)
    bottom_of_core_interval = models.FloatField(null=True)
    sediment_type = models.CharField(max_length=250, null=True)
    reported_sediment_type = models.CharField(max_length=250, null=True)
    sample_volume = models.FloatField(null=True)
    salinity = models.FloatField(null=True)
    temperature = models.FloatField(null=True)
    filtered = models.CharField(max_length=255, null=True)
    mesh_size = models.CharField(max_length=255, null=True)
    sample_surface_area = models.FloatField(null=True)
    dry_weight = models.FloatField(null=True)
    wet_weight = models.FloatField(null=True)
    dry_wet_percentage = models.FloatField(null=True)
    sampling_method = models.CharField(max_length=100, null=True)
    preparation_method = models.CharField(max_length=100, null=True)
    drying_method = models.CharField(max_length=100, null=True)
    counting_method = models.CharField(max_length=100, null=True)
    maris_reference_id = models.CharField(max_length=250, null=True)
    reference_citation = models.TextField(null=True)
    zotero_url = models.URLField(null=True)
    reference_note = models.TextField(null=True)
    sample_note = models.TextField(null=True)
    measurement_note = models.TextField(null=True)
    activity_corrected = models.FloatField(null=True)
    uncertainty_corrected = models.FloatField(null=True)
    unit_corrected = models.CharField(max_length=250, null=True)


# ehh
class SedimentSample(models.Model):
    MARIS_sample_id = models.CharField(max_length=250, null=True)
    sample_type = models.CharField(max_length=250, null=True)
    laboratory = models.CharField(max_length=100, null=True)
    latitude = models.DecimalField(max_digits=9, decimal_places=6, null=True)
    longitude = models.DecimalField(max_digits=9, decimal_places=6, null=True)
    station_id = models.CharField(max_length=250, null=True)
    data_provider_sample_id = models.CharField(max_length=255,null=True)  # Data provider's sample ID
    maris_profile_id = models.CharField(max_length=250, null=True)
    maris_transect_id = models.CharField(max_length=250, null=True)
    sampling_depth = models.FloatField(null=True)
    total_depth = models.FloatField(null=True)
    sampling_start_datetime = models.DateTimeField(null=True)
    sampling_end_datetime = models.DateTimeField(null=True)
    nuclide = models.CharField(max_length=250, null=True)
    value_type = models.CharField(max_length=250, null=True)
    activity_or_mda = models.FloatField(null=True)
    uncertainty = models.FloatField(null=True)
    unit = models.CharField(max_length=250, null=True)
    variable_type = models.CharField(max_length=250, null=True)
    num_data_points = models.IntegerField(null=True)
    min_value = models.FloatField(null=True)
    max_value = models.FloatField(null=True)
    taxon_name = models.CharField(max_length=100, null=True)
    reported_taxon_name = models.CharField(max_length=100, null=True)
    taxon_rank = models.CharField(max_length=250, null=True)
    biogroup = models.CharField(max_length=250, null=True)
    taxon_database_url = models.URLField(null=True)
    body_part = models.CharField(max_length=250, null=True)
    top_of_core_interval = models.FloatField(null=True)
    bottom_of_core_interval = models.FloatField(null=True)
    sediment_type = models.CharField(max_length=250, null=True)
    reported_sediment_type = models.CharField(max_length=50, null=True)
    sample_volume = models.FloatField(null=True)
    salinity = models.FloatField(null=True)
    temperature = models.FloatField(null=True)
    filtered = models.CharField(max_length=255, null=True)
    mesh_size = models.CharField(max_length=255, null=True)
    sample_surface_area = models.FloatField(null=True)
    dry_weight = models.FloatField(null=True)
    wet_weight = models.FloatField(null=True)
    dry_wet_percentage = models.FloatField(null=True)
    sampling_method = models.CharField(max_length=100, null=True)
    preparation_method = models.CharField(max_length=100, null=True)
    drying_method = models.CharField(max_length=100, null=True)
    counting_method = models.CharField(max_length=100, null=True)
    maris_reference_id = models.CharField(max_length=250, null=True)
    reference_citation = models.TextField(null=True)
    zotero_url = models.URLField(null=True)
    reference_note = models.TextField(null=True)
    sample_note = models.TextField(null=True)
    measurement_note = models.TextField(null=True)
    activity_corrected = models.FloatField(null=True)
    uncertainty_corrected = models.FloatField(null=True)
    unit_corrected = models.CharField(max_length=250, null=True)


class DetectionResult(models.Model):
    """
    Detection results table
    """

    detec_res_id = models.AutoField(primary_key=True)
    emp_name = models.CharField(max_length=50, null=True)
    equipment = models.CharField(max_length=50, null=True)
    agency = models.CharField(max_length=50, null=True)
    time = models.DateTimeField(null=True)
    report_id = models.IntegerField()
    rd_entity_id = models.ForeignKey(RadiationSample, on_delete=models.CASCADE)

    def __str__(self):
        """String representation of the detection result."""
        return f"Detection Result {self.detec_res_id}"


class Regions(models.Model):
    """
    Countries Table
    """

    ISO_country = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=50)
    qualification = models.CharField(max_length=50)

    def __str__(self):
        """String representation of the country."""
        return self.name


class CurrentPassing(models.Model):
    """
    Current Passing Table
    """
    ISO_country = models.IntegerField(primary_key=True)
    rd_sub_id = models.ForeignKey(RadioactiveMaterial, on_delete=models.CASCADE)

    def __str__(self):
        """String representation of the country."""
        return f"ISO Country {self.ISO_country}"


class RadiationSourceDistance(models.Model):
    """Distances between radiation sources and monitoring stations Table"""

    rd_sub_id = models.IntegerField(primary_key=True)
    station_id = models.ForeignKey(RadiationMonitoringStation, on_delete=models.CASCADE)
    distance = models.FloatField()

    def __str__(self):
        """String representation of the radiation source distance."""
        return f"Source {self.rd_sub_id} - Station {self.station_id}"


class Register(models.Model):
    user_name = models.CharField(max_length=255)  # Sample type
    password = models.CharField(max_length=255)  # Laboratory
    email = models.EmailField(max_length=255)  # 新增邮箱字段
    role = models.IntegerField()


class Posts(models.Model):
    title = models.CharField(max_length=1000)
    content = models.CharField(max_length=1000)
    time = models.DateTimeField(null=True)  # Sampling start datetime


class WaitingPool(models.Model):
    title = models.CharField(max_length=1000)
    content = models.CharField(max_length=1000)
    time = models.DateTimeField(null=True)  # Sampling start datetime


